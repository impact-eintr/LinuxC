#include "../headers/common.h"
#include "../headers/linker.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_LINK
#ifdef DEBUG_LINK

static void print_sh_entry(sh_entry_t *sh) {
  printf("%s\t%lx\t%lu\t%lu\n", sh->sh_name, sh->sh_addr, sh->sh_offset,
         sh->sh_size);
}

static void print_symtab_entry(st_entry_t *ste) {
  printf("%s\t%d\t%d\t%s\t%lu\t%lu\n", ste->st_name, ste->bind, ste->type,
         ste->st_shndx, ste->st_value, ste->st_size);
}

static void print_relocation_entry(rl_entry_t *rte) {
  printf("%lu\t%lu\t%d\t%u\t%ld\n", rte->r_row, rte->r_col, rte->type, rte->sym, rte->r_addend);
}

#endif

hashtable_t *link_constant_dict;

static int parse_table_entry(char *str, char ***ent) {
  // column0,column1,column2
  // parse line as table entries
  int count_col = 1;
  int len = strlen(str);

  // count columns
  for (int i = 0;i < len;++i) {
    if (str[i] == ',') {
      count_col++;
    }
  }
  // malloc and create list
  char **arr = malloc(count_col * sizeof(char *));
  *ent = arr;

  int col_index = 0;
  int col_width = 0;
  char col_buf[32];
  for (int i = 0;i < len + 1;++i) {
    if (str[i] == ',' || str[i] == '\0') { // pause condition
      assert(col_index < count_col);
      char *col = malloc((col_width + 1) * sizeof(char));
      for (int j = 0;j < col_width;++j) {
        col[j] = col_buf[j];
      }
      col[col_width] = '\0';

      // update
      arr[col_index] = col;
      col_index++;
      col_width = 0;
    } else {
      assert(col_width < 32);
      col_buf[col_width] = str[i];
      col_width++;
    }
  }
  return count_col;
}

static void parse_sh(char *str, sh_entry_t *sh) {
  // sh_name,sh_addr,sh_ofset,sh_size(lines)
  // .text.0x0,4,22
  char **cols;
  int num_cols = parse_table_entry(str, &cols);
  assert(num_cols == 4);

  assert(sh != NULL);
  strcpy(sh->sh_name, cols[0]);
  sh->sh_addr = string2uint(cols[1]);
  sh->sh_offset = string2uint(cols[2]);
  sh->sh_size = string2uint(cols[3]);

  // free memory allocated
  for (int i = 0;i < num_cols;++i) {
    free(cols[i]);
  }
  free(cols);
  cols = NULL;
}

static void parse_symtab(char *str, st_entry_t *ste) {
  // st_name,bind,type,st_shndex,st_value,st_size
  // sum,STB_GLOBAL,STT_FUNCTION,.text,0,22
  char **cols;
  int num_cols = parse_table_entry(str, &cols);
  assert(num_cols == 6);

  assert(ste != NULL);
  strcpy(ste->st_name, cols[0]);
  // select symbol bind
  uint64_t bind_value;
  if (hashtable_get(link_constant_dict, cols[1], &bind_value) == 0) {
    exit(1); // failed
  }
  ste->bind = (st_bind_t)bind_value;

  // select symbol type
  uint64_t type_value;
  if (hashtable_get(link_constant_dict, cols[2], &type_value) == 0) {
    exit(1); // failed
  }
  ste->type = (st_type_t)type_value;

  strcpy(ste->st_shndx, cols[3]);
  ste->st_value = string2uint(cols[4]);
  ste->st_size = string2uint(cols[5]);

  // free memory allocated
  for (int i = 0;i < num_cols;++i) {
    free(cols[i]);
  }
  free(cols);
  cols = NULL;
}

static void parse_relocation(char *str, rl_entry_t *rte) {
  // 4,7,R_X86_64_PC32,0,-4
  char **cols;
  int num_cols = parse_table_entry(str, &cols);
  assert(num_cols == 5);

  assert(rte != NULL);
  rte->r_row = string2uint(cols[0]);
  rte->r_col = string2uint(cols[1]);

  // select relocation type
  uint64_t type_value;
  if (hashtable_get(link_constant_dict, cols[2], &type_value) == 0) {
    printf("relocation type is neiter R_X86_64_32, R_X86_64_PC32, nor R_X86_64");
    exit(0);
  }

  rte->type = (reltype)type_value;
  rte->sym = string2uint(cols[3]);
  uint64_t bitmap = string2uint(cols[4]);
  rte->r_addend = *(int64_t *)&bitmap;

  // free memory allocated
  for (int i = 0;i < num_cols;++i) {
    free(cols[i]);
  }
  free(cols);
  cols = NULL;
}

static int read_elf(const char *filename, uint64_t bufaddr) {
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("file not found\n");
    exit(1);
  }

  // read text file line by line
  char line[MAX_ELF_FILE_WIDTH];
  int line_count = 0;

  while (fgets(line, MAX_ELF_FILE_WIDTH, fp) != NULL) {
    int len = strlen(line);
    if ((len == 0) || (len >= 1 && (line[0] == '\n' || line[0] == '\r')) ||
        (len >= 2 && (line[0] == '/' && line[1] == '/'))) {
      continue;
    }
    int iswhite = 1;
    for (int i = 0; i < len; ++i) {
      iswhite = iswhite && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r');
    }
    if (iswhite == 1) {
      continue;
    }

    if (line_count < MAX_ELF_FILE_LENGTH) {
      uint64_t addr = bufaddr + line_count * MAX_ELF_FILE_WIDTH * sizeof(char);
      char *linebuf = (char *)addr;

      int i = 0;
      while (i < len && i < MAX_ELF_FILE_WIDTH) {
        if ((line[i] == '\n') || (line[i] == '\r') ||
            ((i + 1 < len) && (i + 1 < MAX_ELF_FILE_WIDTH) && line[i] == '/' && line[i + 1] == '/')) {
          break;
        }
        linebuf[i] = line[i];
        i++;
      }
      linebuf[i] = '\0';
      line_count++;
    } else {
      fclose(fp);
      exit(1);
    }
  }

  fclose(fp);
  assert(string2uint((char*)bufaddr) == line_count);
  return line_count;
}

static void init_dictionary() {
  if (link_constant_dict != NULL) {
    return;
  }

  link_constant_dict = hashtable_construct(4);

  link_constant_dict =
      hashtable_insert(link_constant_dict, "STB_LOCAL", STB_LOCAL);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "STB_GLOBAL", STB_GLOBAL);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "STB_WEAK", STB_WEAK);

  link_constant_dict =
      hashtable_insert(link_constant_dict, "STT_NOTYPE", STT_NOTYPE);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "STT_OBJECT", STT_OBJECT);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "STT_FUNC", STT_FUNC);

  link_constant_dict =
      hashtable_insert(link_constant_dict, "R_X86_64_32", R_X86_64_32);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "R_X86_64_PC32", R_X86_64_PC32);
  link_constant_dict =
      hashtable_insert(link_constant_dict, "R_X86_64_PLT32", R_X86_64_PLT32);
  //print_hashtable(link_constant_dict);
}

void parse_elf(char *filename, elf_t *elf) {
  assert(elf != NULL);
  elf->line_count = read_elf(filename, (uint64_t)&(elf->buffer));
  for (int i = 0;i < elf->line_count;++i) {
    printf("[%d]\t%s\n", i, elf->buffer[i]);
  }
  init_dictionary();

  // parse section headers
  elf->sht_count = string2uint(elf->buffer[1]);
  elf->sht = malloc(elf->sht_count * sizeof(sh_entry_t));
  memset(elf->sht, 0, elf->sht_count * sizeof(sh_entry_t));

  sh_entry_t *rtext_sh = NULL;
  sh_entry_t *rdata_sh = NULL;
  sh_entry_t *symt_sh = NULL;
  for (int i = 0;i < elf->sht_count;++i) {
    parse_sh(elf->buffer[2+i], &(elf->sht[i]));
#ifdef DEBUG_LINK
    print_sh_entry(&(elf->sht[i]));
#endif
    if (strcmp(elf->sht[i].sh_name, ".rel.text") == 0) {
      rtext_sh = &(elf->sht[i]);
    } else if(strcmp(elf->sht[i].sh_name, ".rel.data") == 0) {
      rdata_sh = &(elf->sht[i]);
    } else if(strcmp(elf->sht[i].sh_name, ".symtab") == 0) {
      symt_sh = &(elf->sht[i]);
    }
  }
  assert(symt_sh != NULL);

  // parse symbol table
  elf->symt_count = symt_sh->sh_size;
  elf->symt = malloc(elf->symt_count * sizeof(st_entry_t));
  memset(elf->symt, 0, elf->symt_count * sizeof(st_entry_t));

  for (int i = 0; i < symt_sh->sh_size; ++i) {
    parse_symtab(elf->buffer[i + symt_sh->sh_offset], &(elf->symt[i]));
#ifdef DEBUG_LINK
    print_symtab_entry(&(elf->symt[i]));
#endif
  }

  // parse relocation table
  // .rel.text
  if (rtext_sh != NULL) {
    elf->reltext_count = rtext_sh->sh_size;
    elf->reltext = malloc(elf->reltext_count * sizeof(rl_entry_t));
    memset(elf->reltext, 0, elf->reltext_count * sizeof(rl_entry_t));
    for (int i = 0;i < rtext_sh->sh_size;++i) {
      parse_relocation(elf->buffer[i+rtext_sh->sh_offset], &(elf->reltext[i]));
      int st = elf->reltext[i].sym;
      assert(0 <= st && st < elf->symt_count);
#ifdef DEBUG_LINK
      print_relocation_entry(&(elf->reltext[i]));
#endif
    }
  } else {
    elf->reltext_count = 0;
    elf->reltext = NULL;
  }
  // .rel.data
  if (rdata_sh != NULL) {
    elf->reldata_count = rdata_sh->sh_size;
    elf->reldata = malloc(elf->reldata_count * sizeof(rl_entry_t));
    memset(elf->reldata, 0, elf->reldata_count * sizeof(rl_entry_t));

    for (int i = 0; i < rdata_sh->sh_size; ++i) {
      parse_relocation(elf->buffer[i + rdata_sh->sh_offset], &(elf->reldata[i]));
      int st = elf->reldata[i].sym;
      assert(0 <= st && st < elf->symt_count);
#ifdef DEBUG_LINK
      print_relocation_entry(&(elf->reldata[i]));
#endif
    }
  } else {
    elf->reldata_count = 0;
    elf->reldata = NULL;
  }
}

void write_eof(const char *filename, elf_t *eof) {
  // open elf file
  FILE *fp;
  fp = fopen(filename, "w");
  if (fp == NULL) {
#ifdef DEBUG_LINK
    printf("unable to open file %s\n", filename);
#endif
    exit(1);
  }

  for (int i = 0; i < eof->line_count; ++i) {
    fprintf(fp, "%s\n", eof->buffer[i]);
  }

  fclose(fp);

  // free hash table
  hashtable_free(link_constant_dict);
}

void free_elf(elf_t *elf) {
  assert(elf != NULL);

  if (elf->sht != NULL) {
    free(elf->sht);
  }

  if (elf->symt != NULL) {
    free(elf->symt);
  }

  if (elf->reltext != NULL) {
    free(elf->reltext);
  }

  if (elf->reldata != NULL) {
    free(elf->reldata);
  }

  free(elf);
}

#define DEBUG_LINK
#ifdef DEBUG_LINK
int main() {
  elf_t *elf_arr[2];
  elf_arr[0] = malloc(sizeof(elf_t));
  elf_arr[1] = malloc(sizeof(elf_t));

  parse_elf("../file/sum.elf.txt", elf_arr[0]);
  parse_elf("../file/main.elf.txt", elf_arr[1]);

  elf_t dst;
  link_elf((elf_t**)&elf_arr, 2, &dst);

  free_elf(elf_arr[0]);
  free_elf(elf_arr[1]);

  write_eof("../file/eof.txt", &dst);
}
#endif
