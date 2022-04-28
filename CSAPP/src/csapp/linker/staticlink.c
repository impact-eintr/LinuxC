#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/common.h"
#include "../headers/instruction.h"
#include "../headers/linker.h"

#define MAX_SYMBOL_MAP_LENGTH (64)
#define MAX_SECTION_BUFFER_LENGTH (64)
#define MAX_RELOCATION_LINES (64)

typedef struct {
  elf_t *elf; // src elf files: main.c sum.o ...
  st_entry_t *src; // src symbols: unique strong_symbol table
  st_entry_t *dst; // dst symbols: destination file symbol table after recaculated offset
} smap_t; // symbol map type

/* ------------------------------------ */
/* Symbol Processing                    */
/* ------------------------------------ */
static void symbol_processing(elf_t **srcs, int num_srcs, elf_t *dst,
                              smap_t *smap_table, int *smap_count);
static void simple_resolution(st_entry_t *sym, elf_t *sym_elf, smap_t *candidate);

/* ------------------------------------ */
/* Section Merging                      */
/* ------------------------------------ */
static void compute_section_header(elf_t *dst, smap_t *smap_table, int *smap_count);
static void merge_section(elf_t **srcs, int num_srcs, elf_t *dst,
                          smap_t *smap_table, int *smap_count);

/* ------------------------------------ */
/* Relocation                           */
/* ------------------------------------ */
static void relocation_processing(elf_t **srcs, int num_srcs, elf_t *dst,
                                  smap_t *smap_table, int *smap_count);

static void R_X86_64_32_handler(elf_t *dst, sh_entry_t *sh,
                                int row_referencing, int col_referencing, int addend,
                                st_entry_t *sym_referenced);
static void R_X86_64_PC32_handler(elf_t *dst, sh_entry_t *sh,
                                  int row_referencing, int col_referencing, int addend,
                                  st_entry_t *sym_referenced);

typedef void (*rela_handler_t)(elf_t *dst, sh_entry_t *sh,
                               int row_referencing, int col_referencing, int addend,
                               st_entry_t *sym_referenced);
static rela_handler_t handler_table[3] = {
  &R_X86_64_32_handler,       // 0
  &R_X86_64_PC32_handler,     // 1
  // linux commit b21ebf2: x86: Treat R_X86_64_PLT32 as R_X86_64_PC32
  // https://github.com/torvalds/linux/commit/b21ebf2fb4cde1618915a97cc773e287ff49173e
  &R_X86_64_PC32_handler,     // 2
};

/* ------------------------------------ */
/* Helper                               */
/* ------------------------------------ */
static const char *get_stb_string(st_bind_t bind);
static const char *get_stt_string(st_type_t type);

/* ------------------------------------ */
/* Exposed Interface for Static Linking */
/* ------------------------------------ */
#define DEBUG_LINK
void link_elf(elf_t **srcs, int num_srcs, elf_t *dst) {
  memset(dst, 0, sizeof(elf_t));
  int smap_count = 0;
  smap_t smap_table[MAX_SYMBOL_MAP_LENGTH];

  // symbol resolution
  symbol_processing(srcs, num_srcs, dst, smap_table, &smap_count);
#ifdef DEBUG_LINK
  printf("----------------------\n");
  for (int i = 0; i < smap_count; ++i) {
    st_entry_t *ste = smap_table[i].src;
    printf("%s\t%d\t%d\t%s\t%ld\t%ld\n", ste->st_name, ste->bind, ste->type,
           ste->st_shndx, ste->st_value, ste->st_size);
  }
#endif

  compute_section_header(dst, smap_table, &smap_count);

  // malloc the dst.symt
  dst->symt_count = smap_count; // 4 : array bias sum main
  dst->symt = malloc(dst->symt_count * sizeof(st_entry_t));

  // to this point, the EOF file header and section header table is placed
  // merge the left sections and relocate the entries in .text and .data

  // merge the symbol content from ELF src into dst sections
  merge_section(srcs, num_srcs, dst, smap_table, &smap_count);

#ifdef DEBUG_LINK
  printf("----------------------\n");
  printf("after merging the sections\n");
  for (int i = 0; i < dst->line_count; ++i) {
    printf("%s\n", dst->buffer[i]);
  }
#endif

  // relocating: update the relocation entries from ELF files into EOF buffer
  relocation_processing(srcs, num_srcs, dst, smap_table, &smap_count);

#ifdef DEBUG_LINK
  // finally, check EOF file
  printf("-----------------------\nfinal output EOF:\n");
  for (int i = 0; i < dst->line_count; ++i) {
    printf("%s\n", dst->buffer[i]);
  }
#endif
}

static void symbol_processing(elf_t **srcs, int num_srcs, elf_t *dst,
                              smap_t *smap_table, int *smap_count) {
  for (int i = 0;i < num_srcs;++i) {
    elf_t *elfp = srcs[i];

    for (int j = 0;j < elfp->symt_count;++j) {
      st_entry_t *sym = &(elfp->symt[j]);

      if (sym->bind == STB_LOCAL) {
        // local symbol is independent and NOTE we have not created dst here
        assert(*smap_count < MAX_SYMBOL_MAP_LENGTH);
        smap_table[*smap_count].src = sym;
        smap_table[*smap_count].elf = elfp;
        (*smap_count)++;
      } else if (sym->bind == STB_GLOBAL) {
        for (int k = 0;k < *smap_count;++k) {
          st_entry_t *candidate = smap_table[k].src;
          if (candidate->bind == STB_GLOBAL &&
              (strcmp(candidate->st_name, sym->st_name) == 0)) {
            // resolve conflict
            simple_resolution(sym, elfp, &smap_table[k]);
            goto NEXT_SYMBOL_PROCESS;
          }
        }

        assert(*smap_count <= MAX_SYMBOL_MAP_LENGTH);
        smap_table[*smap_count].src = sym;
        smap_table[*smap_count].elf = elfp;
        (*smap_count)++;
      }
NEXT_SYMBOL_PROCESS:
      // do something
      ;
    }
  }

  // cleanup: check if there is any undefined symbols in the map table
  for (int i = 0;i < *smap_count;++i) {
    st_entry_t *s = smap_table[i].src;

    // check no more SHN_DEF here
    assert(strcmp(s->st_shndx, "SHN_UNDEF") != 0);
    assert(s->type != STT_NOTYPE);
    // the remaining COMMON go to .bss
    if (strcmp(s->st_shndx, "COMMON") == 0) {
      char *bss = ".bss";
      for (int j = 0; j < MAX_CHAR_SECTION_NAME; ++j) {
        if (j < 4) {
          s->st_shndx[j] = bss[j];
        } else {
          s->st_shndx[j] = '\0';
        }
      }
      s->st_value = 0;
    }
  }
}

static inline int symbol_precedence(st_entry_t *sym) {
  // use inline function to imporve efficiency in run-time by preprocessing
  /*  we do not consider weak because it's very rare
      and we do not consider local because it's not conflicting

          bind        type        shndx               prec
          --------------------------------------------------
          global      notype      undef               0 - undefined
          global      object      common              1 - tentative
          global      object      data,bss,rodata     2 - defined
          global      func        text                2 - defined
  */
  assert(sym->bind == STB_GLOBAL);

  // get precedence of the symbol

  if (strcmp(sym->st_shndx, "SHN_UNDEF") == 0 && sym->type == STT_NOTYPE) {
    // Undefined: symbols referenced but not assigned a storage address
    return 0;
  }
  if (strcmp(sym->st_shndx, "COMMON") == 0 && sym->type == STT_OBJECT) {
    // Tentative: section to be decided after symbol resolution
    return 1;
  }
  if ((strcmp(sym->st_shndx, ".text") == 0 && sym->type == STT_FUNC) ||
      (strcmp(sym->st_shndx, ".data") == 0 && sym->type == STT_OBJECT) ||
      (strcmp(sym->st_shndx, ".rodata") == 0 && sym->type == STT_OBJECT) ||
      (strcmp(sym->st_shndx, ".bss") == 0 && sym->type == STT_OBJECT)) {
    // Defined
    return 2;
  }

  printf("symbol resolution: cannot determine the symbol \"%s\" precedence", sym->st_name);
  exit(0);
}

static void simple_resolution(st_entry_t *sym, elf_t *sym_elf, smap_t *candidate) {
  // rule1: mutiple strong symbols with the same name are not allowed
  // rule2: given a strong symbol and mutiple weak symbols whith the same name,choose the strong symbol
  // rule3: given multiple weak symbols with the same name, choose any of the weak symbols
  int pre1 = symbol_precedence(sym);
  int pre2 = symbol_precedence(candidate->src);

  if (pre1 == 2 && pre2 == 2) {
    printf("symbol resolution: strong symbol \"%s\" is redeclared\n",
           sym->st_name);
    exit(0);
  } else if (pre1 != 2 && pre2 != 2) {
    if (pre1 > pre2) {
      candidate->src = sym;
      candidate->elf = sym_elf;
    }
    return;
  } else if (pre1 == 2) {
    candidate->src = sym;
    candidate->elf = sym_elf;
  }
}

static void compute_section_header(elf_t *dst, smap_t *smap_table, int *smap_count) {
  // we have .text, .rodata, .data as symbols in the section
  // .bss is not taking any section memory
  int count_text = 0, count_rodata = 0, count_data = 0;
  for (int i = 0;i < *smap_count;++i) {
    st_entry_t *sym = smap_table[i].src;
    if (strcmp(sym->st_shndx, ".text") == 0) {
      count_text += sym->st_size;
    } else if (strcmp(sym->st_shndx, ".rodata") == 0) {
      count_rodata += sym->st_size;
    } else if (strcmp(sym->st_shndx, ".data") == 0) {
      count_data += sym->st_size;
    }
  }

  // count the section: with .symtab
  dst->sht_count = (count_text != 0) + (count_rodata != 0) + (count_data != 0) + 1;
  // count the total lines
  dst->line_count = 1 + 1 + dst->sht_count + count_text + count_rodata + count_data + *smap_count;
  // the target dst: line_count'\n'sht_count'\n'sht'\n'.text rodata .data .symbol
  sprintf(dst->buffer[0], "%ld", dst->line_count);
  sprintf(dst->buffer[1], "%ld", dst->sht_count);

  // compute the run-time addres of the sections:compact in memory
  uint64_t text_runtime_addr = 0x00400000;
  uint64_t rodata_runtime_addr = text_runtime_addr + count_text * MAX_INSTRUCTION_CHAR * sizeof(char);
  uint64_t data_runtime_addr = rodata_runtime_addr + count_rodata * sizeof(uint64_t);
  uint64_t symtab_runtime_addr = 0; // For EOF, .symtab is not loaded into run-time memory but still on disk

  // write the section header table
  assert(dst->sht == NULL);
  dst->sht = malloc(dst->sht_count * sizeof(sh_entry_t));

  // write in .text .rodata .data order
  uint64_t section_offset = 1 + 1 + dst->sht_count;
  int sh_index = 0;
  sh_entry_t *sh = NULL;

  if (count_text > 0) {
    assert(sh_index < dst->sht_count);
    sh = &(dst->sht[sh_index]);

    // write the fields
    strcpy(sh->sh_name, ".text");
    sh->sh_addr = text_runtime_addr;
    sh->sh_offset = section_offset;
    sh->sh_size = count_text;

    // write to buffer
    sprintf(dst->buffer[2 + sh_index], "%s,0x%lx,%ld,%ld", sh->sh_name,
            sh->sh_addr, sh->sh_offset, sh->sh_size);
    sh_index++;
    section_offset += sh->sh_size;
  }
  if (count_rodata > 0) {
    assert(sh_index < dst->sht_count);
    sh = &(dst->sht[sh_index]);

    // write the fields
    strcpy(sh->sh_name, ".rodata");
    sh->sh_addr = rodata_runtime_addr;
    sh->sh_offset = section_offset;
    sh->sh_size = count_rodata;

    // write to buffer
    sprintf(dst->buffer[2 + sh_index], "%s,0x%lx,%ld,%ld", sh->sh_name,
            sh->sh_addr, sh->sh_offset, sh->sh_size);

    sh_index++;
    section_offset += sh->sh_size;
  }
  if (count_data > 0) {
    assert(sh_index < dst->sht_count);
    sh = &(dst->sht[sh_index]);

    // write the fields
    strcpy(sh->sh_name, ".data");
    sh->sh_addr = data_runtime_addr;
    sh->sh_offset = section_offset;
    sh->sh_size = count_data;

    sprintf(dst->buffer[2 + sh_index], "%s,0x%lx,%ld,%ld", sh->sh_name,
            sh->sh_addr, sh->sh_offset, sh->sh_size);
    sh_index++;
    section_offset += sh->sh_size;
  }
  // .symbol
  assert(sh_index < dst->sht_count);
  sh = &(dst->sht[sh_index]);

  // write the fields
  strcpy(sh->sh_name, ".symbol");
  sh->sh_addr = symtab_runtime_addr;
  sh->sh_offset = section_offset;
  sh->sh_size = *smap_count;
  // write to buffer
  sprintf(dst->buffer[2 + sh_index], "%s,0x%lx,%ld,%ld", sh->sh_name,
          sh->sh_addr, sh->sh_offset, sh->sh_size);

  assert(sh_index + 1 == dst->sht_count);
  // print and check
#ifdef DEBUG_LINK
  printf("----------------------\n");
  printf("Destination ELF's SHT in Buffer:\n");
  for (int i = 0; i < 2 + dst->sht_count; ++i) {
    printf("%s\n", dst->buffer[i]);
  }
#endif
}

static void merge_section(elf_t **srcs, int num_srcs, elf_t *dst,
                          smap_t *smap_table, int *smap_count) {
  int line_written = 1 + 1 + dst->sht_count;
  int symt_written = 0;
  int sym_section_offset = 0;

  // Traverse all the merged section tables, find the section containing the
  // section name in all elf files, and merge it into the corresponding section
  // of the target file
  for (int section_index = 0; section_index < dst->sht_count; ++section_index) {
    sh_entry_t *target_sh = &dst->sht[section_index];
    sym_section_offset = 0;
#ifdef DEBUG_LINK
    printf("merging section '%s'\n", target_sh->sh_name);
#endif
    // merge the sections from sht
    for (int i = 0; i < num_srcs; ++i) {
#ifdef DEBUG_LINK
      printf("\tfrom source elf [%d]\n", i);
#endif
      int src_section_index = -1;
      for (int j = 0; j < srcs[i]->sht_count; ++j) {
        if (strcmp(target_sh->sh_name, srcs[i]->sht[j].sh_name) ==
            0) { // found the target merge section
          src_section_index = j;
          // break;
        }
      }
      if (src_section_index == -1) {
        continue;
      } else {
        // let's merge it!
        for (int j = 0; j < srcs[i]->symt_count; ++j) {
          st_entry_t *sym = &srcs[i]->symt[j];

          // if the current file contains the current merged section and
          // there is an entry in the symbol table that points to the merged
          // section name
          if (strcmp(sym->st_shndx, target_sh->sh_name) == 0) {
            for (int k = 0; k < *smap_count; ++k) {
              if (sym == smap_table[k].src) {
#ifdef DEBUG_LINK
                printf("\t\tsymbol '%s'\n", sym->st_name);
#endif
                // NOTE: we will filling the "dst" and smap_table[k]
                for (int t = 0; t < sym->st_size; ++t) {
                  // copy section to segment
                  int dst_index = line_written + t;
                  int src_index = srcs[i]->sht[src_section_index].sh_offset + sym->st_value + t;
                  assert(dst_index < MAX_ELF_FILE_LENGTH);
                  assert(src_index < MAX_ELF_FILE_LENGTH);
                  strcpy(dst->buffer[dst_index], srcs[i]->buffer[src_index]);
                }
                // copy the symbol table entry from srcs[i].symt[j] to
                // dst.symt[symt_written]
                assert(symt_written < dst->symt_count);
                // copy the entry
                strcpy(dst->symt[symt_written].st_name, sym->st_name);
                dst->symt[symt_written].bind = sym->bind;
                dst->symt[symt_written].type = sym->type;
                strcpy(dst->symt[symt_written].st_shndx, sym->st_shndx);
                // NOTE: recalculate offset: sym->st_value to sy_section_offset
                dst->symt[symt_written].st_value = sym_section_offset;
                dst->symt[symt_written].st_size = sym->st_size;

                // update the smap_table
                // this will help the relocation
                smap_table[k].dst = &dst->symt[symt_written];

                // udpate the counter
                symt_written += 1;
                line_written += sym->st_size;
                sym_section_offset += sym->st_size;
              }
            }
          }
        }
      }
    }
  }
  // finally merge .symtab form dst->symtab
  for (int i = 0; i < dst->symt_count; ++i) {
    st_entry_t *sym = &dst->symt[i];
    sprintf(dst->buffer[line_written], "%s,%s,%s,%s,%ld,%ld", sym->st_name,
            get_stb_string(sym->bind), get_stt_string(sym->type), sym->st_shndx,
            sym->st_value,
            sym->st_size); // st_value, st_size : offset and size in the section
    line_written++;
  }
  assert(line_written == dst->line_count);
}

// precondition: smap_table.dst is valid
static void relocation_processing(elf_t **srcs, int num_srcs, elf_t *dst,
                                  smap_t *smap_table, int *smap_count) {
  sh_entry_t *eof_text_sh = NULL;
  sh_entry_t *eof_data_sh = NULL;
  for (int i = 0;i < dst->sht_count;++i) {
    if (strcmp(dst->sht[i].sh_name, ".text") == 0) {
      eof_text_sh = &(dst->sht[i]);
    } else if (strcmp(dst->sht[i].sh_name, ".data") == 0) {
      eof_data_sh = &(dst->sht[i]);
    }
  }

  // update the relocation entries:r_row, r_col, sym
  for (int i = 0;i < num_srcs;++i) {
    elf_t *elf = srcs[i];
    // .rel.text
    for (int j = 0;j < elf->reltext_count;++j) {
      rl_entry_t *r = &elf->reltext[j];

      // search the referencing symbol
      for (int k = 0;k < elf->symt_count;++k) {
        st_entry_t *sym = &elf->symt[k];
        //printf("test 文件中的reltext section 定位 symtab %s\n", sym->st_name);

        if (strcmp(sym->st_shndx, ".text") == 0) {
          int sym_text_start = sym->st_value;
          int sym_text_end = sym_text_start + sym->st_size - 1;
          //printf("遍历文件中的reltext section 发现symtab中的%s 的所属section 为.text\n", sym->st_name);
          if (sym_text_start <= r->r_row && r->r_row <= sym_text_end) {
            // [sym_text_start, sym_text_end]
            //printf(
            //    "遍历文件的reltext 当前重定位目标是 %s 发现其symtab中%s->sh_shndx 为.text\
 区间吻合 则//%s位于%s的第[%ld]行\n",
            //    elf->symt[r->sym].st_name, sym->st_name,
            //    elf->symt[r->sym].st_name, sym->st_name,
            //    r->r_row);
            int smap_found = 0;
            // next,start modifying the segment that needs to be relocation
            for (int t = 0; t < *smap_count; ++t) {
              if (smap_table[t].src == sym) {
                smap_found = 1;
                st_entry_t *eof_referencing = smap_table[t].dst; // smap_table[t].dst is the point to segment conten
                // search the being referenced symbol, UPDATE THE SEGMENT
                for (int u = 0;u < *smap_count;++u) {
                  if (strcmp(elf->symt[r->sym].st_name, smap_table[u].dst->st_name) == 0 &&
                    smap_table[u].dst->bind == STB_GLOBAL) {
                    st_entry_t *eof_referenced = smap_table[u].dst;
                    (handler_table[(int)r->type])(dst, eof_text_sh,
                      r->r_row - sym->st_value + eof_referencing->st_value,
                      r->r_col, r->r_addend, eof_referenced);
                    goto NEXT_REFERENCE_IN_TEXT;
                  }
                }
              }
            }
            assert(smap_found == 1);
          }
        }
      }
NEXT_REFERENCE_IN_TEXT:
      ;
    }
    // .rel.data
    for (int j = 0;j < elf->reldata_count;j++) {
      rl_entry_t *r = &elf->reldata[j];

      // search the referencing symbol
      for (int k = 0;k < elf->symt_count;++k) {
        st_entry_t *sym = &elf->symt[k];
        //printf("test 文件中的reldata section 定位 symtab %s\n", sym->st_name);

        if (strcmp(sym->st_shndx, ".data") == 0) {
          int sym_data_start = sym->st_value;
          int sym_data_end = sym_data_start + sym->st_size - 1;
          //printf("遍历文件中的reldata section 发现symtab中的%s 的所属section 为.data\n", sym->st_name);
          if (sym_data_start <= r->r_row && r->r_row <= sym_data_end) {
            // [sym_data_start, sym_data_end]
            //printf(
            //    "遍历文件的reldata 当前重定位目标是 %s 发现其symtab中%s->sh_shndx 为.data\
 区间吻合 则//%s位于%s的第[%ld]行\n",
            //    elf->symt[r->sym].st_name, sym->st_name,
            //    elf->symt[r->sym].st_name, sym->st_name,
            //    r->r_row);
            int smap_found = 0;
            // next,start modifying the segment that needs to be relocation
            for (int t = 0; t < *smap_count; ++t) {
              if (smap_table[t].src == sym) {
                smap_found = 1;
                st_entry_t *eof_referencing = smap_table[t].dst; // smap_table[t].dst is the point to segment conten
                // search the being referenced symbol, UPDATE THE SEGMENT
                for (int u = 0;u < *smap_count;++u) {
                  if (strcmp(elf->symt[r->sym].st_name, smap_table[u].dst->st_name) == 0 &&
                    smap_table[u].dst->bind == STB_GLOBAL) {
                    st_entry_t *eof_referenced = smap_table[u].dst;
                    (handler_table[(int)r->type])(dst, eof_data_sh,
                      r->r_row - sym->st_value + eof_referencing->st_value,
                      r->r_col, r->r_addend, eof_referenced);
                    goto NEXT_REFERENCE_IN_DATA;
                  }
                }
              }
            }
            assert(smap_found == 1);
          }
        }
      }
NEXT_REFERENCE_IN_DATA:
      ;
    }
  }
}

// relocating handlers

static uint64_t get_symbol_runtime_address(elf_t *dst, st_entry_t *sym) {
  // get the tun-time address of symbol
  uint64_t base = 0x00400000;

  uint64_t text_base = base;
  uint64_t rodata_base = base;
  uint64_t data_base = base;

  int inst_size = sizeof(inst_t);
  int data_size = sizeof(uint64_t);

  sh_entry_t *sht = dst->sht;
  for (int i = 0;i < dst->sht_count;++i) {
    if (strcmp(sht[i].sh_name, ".text") == 0) {
      rodata_base = text_base + sht[i].sh_size * inst_size;
      data_base = rodata_base;
    } else if (strcmp(sht[i].sh_name, ".rodata") == 0) {
      data_base = rodata_base + sht[i].sh_size * inst_size;
    }
  }
  // check this symbol's section
  if (strcmp(sym->st_shndx, ".text") == 0) {
    return text_base + inst_size * sym->st_value;
  } else if (strcmp(sym->st_shndx, ".rodata") == 0) {
    return rodata_base + data_size * sym->st_value;
  } else if (strcmp(sym->st_shndx, ".data") == 0) {
    return data_base + data_size * sym->st_value;
  }

  return 0xFFFFFFFFFFFFFFFF;
}

static void write_relocation(char *dst, uint64_t val) {
  char temp[20];
  sprintf(temp, "0x%016lx", val);
  for (int i = 0; i < 18; ++i) {
    dst[i] = temp[i];
  }
}

static void R_X86_64_32_handler(elf_t *dst, sh_entry_t *sh, int row_referencing,
                                int col_referencing, int addend,
                                st_entry_t *sym_referenced) {
  uint64_t sym_address = get_symbol_runtime_address(dst, sym_referenced);
  // rewrite the relocated address
  char *s = &dst->buffer[sh->sh_offset + row_referencing][col_referencing];
  write_relocation(s, sym_address);
}

static void R_X86_64_PC32_handler(elf_t *dst, sh_entry_t *sh,
                                  int row_referencing, int col_referencing,
                                  int addend, st_entry_t *sym_referenced) {
  assert(strcmp(sh->sh_name, ".text") == 0);

  uint64_t sym_address = get_symbol_runtime_address(dst, sym_referenced);
  printf("[%s] run-time address: %lx\n",sym_referenced->st_name  ,sym_address);
  uint64_t rip_value = 0x00400000 + (row_referencing + 1) * sizeof(inst_t);
  // rewrite the relocated address
  char *s = &dst->buffer[sh->sh_offset + row_referencing][col_referencing];
  write_relocation(s, sym_address - rip_value);
}

static const char *get_stb_string(st_bind_t bind) {
  switch (bind) {
  case STB_GLOBAL:
    return "STB_GLOBAL";
  case STB_LOCAL:
    return "STB_LOCAL";
  case STB_WEAK:
    return "STB_WEAK";
  default:
    printf("incorrect symbol bind\n");
    exit(0);
  }
}

static const char *get_stt_string(st_type_t type) {
  switch (type) {
  case STT_NOTYPE:
    return "STT_NOTYPE";
  case STT_OBJECT:
    return "STT_OBJECT";
  case STT_FUNC:
    return "STT_FUNC";
  default:
    printf("incorrect symbol type\n");
    exit(0);
  }
}
