#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../headers/memory.h"
#include "../../headers/cpu.h"
#include "../../headers/common.h"
#include "../../headers/address.h"

void set_pagemap_swapaddr(uint64_t ppn, uint64_t swap_address);

#define SWAP_PAGE_FILE_LINES (512)
#define SWAP_ADDRESS_MIN (100)

// disk address counter
static char *SWAP_FILE_DIR = "/tmp";
static uint64_t internal_swap_addr = SWAP_ADDRESS_MIN;

uint64_t allocate_swappage(uint64_t ppn) {
  uint64_t saddr = internal_swap_addr++;

  char filename[128];
  sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIR, saddr);
  printf("%s\n", filename);
  FILE *fw = fopen(filename, "w");
  assert(fw != NULL);

  // write zero page for anoymous page
  fclose(fw);
  uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
  memset(&pm[ppn_ppo], 0, PAGE_SIZE);

  set_pagemap_swapaddr(ppn, saddr);

  return saddr;
}

int swap_in(uint64_t saddr, uint64_t ppn) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);

  FILE *fr = NULL;
  char filename[128];

  if (saddr == 0) {
    allocate_swappage(ppn);
    return 0;
  }

  assert(saddr >= SWAP_ADDRESS_MIN);
  sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIR, saddr);
  fr = fopen(filename, "r");
  assert(fr != NULL);

  uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
  char buf[64] = {'0'};
  for  (int i = 0;i < SWAP_PAGE_FILE_LINES;++i) {
    char *str = fgets(buf, 64, fr);
    *((uint64_t *)(&pm[ppn_ppo + i * 8])) = string2uint(str);
  }
  fclose(fr);
  return 1;
}

int swap_out(uint64_t saddr, uint64_t ppn) {
  assert(0 <= ppn && ppn <= MAX_NUM_PHYSICAL_PAGE);
  assert(saddr >= SWAP_ADDRESS_MIN);

  FILE *fw = NULL;
  char filename[128];
  sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIR, saddr);
  fw = fopen(filename, "w");
  assert(fw != NULL);

  uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
  for (int i = 0;i < SWAP_PAGE_FILE_LINES;++i) {
    fprintf(fw, "0x%16lx\n",*((uint64_t *)(&pm[ppn_ppo + i * 8])));
  }
  fclose(fw);
  return 0;
}
