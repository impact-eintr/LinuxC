#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "../headers/memory.h"
#include "../headers/address.h"
#include "../headers/common.h"
#include "../headers/interrupt.h"
#include "../headers/cpu.h"


// swap in / out
int swap_in(uint64_t saddr, uint64_t ppn);
int swap_out(uint64_t saddr, uint64_t ppn);

// physical page descriptor
typedef struct {
  int allocated;
  int dirty;
  int time; // LRU cache: 0 - Fresh

  pte4_t *pte4;
  uint64_t saddr;
} pd_t;

static pd_t page_map[MAX_NUM_PHYSICAL_PAGE];

// get the level 4 page table entry
static pte4_t* get_entry(pte123_t *pgd, address_t *vaddr) {
  int vpns[4] = {
      vaddr->vpn1,
      vaddr->vpn2,
      vaddr->vpn3,
      vaddr->vpn4,
  };
  assert(pgd != NULL);
  assert(sizeof(pte123_t) == sizeof(pte4_t));

  int level = 0;
  pte123_t *tab = pgd;
  while(level < 4) {
    int vpn = vpns[level];
    if (tab[vpn].present != 1) {
      // TODO allocate a new page for next level
      pte123_t *new_tab =
          (pte123_t *)KERNEL_malloc(PAGE_TABLE_ENTRY_NUM * sizeof(pte123_t));
      // .paddr field is 50 bits
      tab[vpn].paddr = (uint64_t)new_tab;
      tab[vpn].present = 1;
    }
    // move to next level
    tab = (pte123_t *)((uint64_t)tab[vpn].paddr);
    level += 1;
  }
  pte4_t *pt = (pte4_t *)tab;
  return &pt[vaddr->vpn4];
}

void page_map_init() {
  for (int k = 0;k < MAX_NUM_PHYSICAL_PAGE;++k) {
    page_map[k].allocated = 0;
    page_map[k].dirty = 0;
    page_map[k].time = 0;
    page_map[k].pte4 = NULL;
  }
}

void pagemap_update_time(uint64_t ppn) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
  assert(page_map[ppn].allocated == 1);
  assert(page_map[ppn].pte4->present == 1);
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE;++i) {
    page_map[i].time += 1;
  }
  page_map[ppn].time = 0;
}

void pagemap_dirty(uint64_t ppn) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
  assert(page_map[ppn].allocated == 1);
  assert(page_map[ppn].pte4->present == 1);
  page_map[ppn].dirty = 1;
  page_map[ppn].pte4->dirty = 1;
}

// used by frame swap-in from swap space
void set_pagemap_swapaddr(uint64_t ppn, uint64_t swap_address) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
  page_map[ppn].saddr = swap_address;
}

void map_pte4(pte4_t *pte, uint64_t ppn) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
  assert(page_map[ppn].allocated == 0);
  assert(page_map[ppn].pte4->present == 0);
  assert(page_map[ppn].pte4 == NULL);

  // Let's consider this, where can we store the swap adress on disk?
  // In this case of physical page being allocated and papped,
  // the swap address is store in reversed maping array
  page_map[ppn].saddr = pte->saddr;

  // map the level 4 page table
  pte->present = 1;
  pte->ppn = ppn;
  pte->dirty = 0;
  // reversed mapping
  page_map[ppn].allocated = 1;
  page_map[ppn].dirty = 0;
  page_map[ppn].time = 0;
  page_map[ppn].pte4 = pte;
}

void unmap_pte4(uint64_t ppn) {
  assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
  assert(page_map[ppn].allocated == 1);
  pte4_t *pte = page_map[ppn].pte4;
  assert(pte->present == 1);

  pte->pte_value = 0;
  pte->present = 0;
  pte->saddr = page_map[ppn].saddr;

  // reversed mapping
  page_map[ppn].allocated = 0;
  page_map[ppn].dirty = 0;
  page_map[ppn].time = 0;
  page_map[ppn].pte4 = NULL;
}

void fix_pagefault() {
  // TODO
}
