#include "../headers/common.h"
#include "../headers/linker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  printf("%lu\t%lu\t%d\t%u\t%ld\n", rte->r_row, rte->r_col, rte->type, rte->sym,
         rte->r_addend);
}

#endif

static int parse_table_entry(char *str, char ***ent) {

}

static void parse_sh(char *str, sh_entry_t *sh) {

}

static void parse_symtab(char *str, st_entry_t *ste) {

}

static void parse_relocation(char *str, rl_entry_t *rte) {

}

static int read_elf(const char *filename, uint64_t bufaddr) {

}

static void init_dictionary() {

}

void parse_elf(char *filename, elf_t *elf) {

}

void write_eof(const char *filename, elf_t *eof) {

}

void free_elf(elf_t *elf) {

}
