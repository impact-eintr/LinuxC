#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "../headers/memory.h"
#include "../headers/address.h"
#include "../headers/common.h"
#include "../headers/interrupt.h"
#include "../headers/cpu.h"
#include "../headers/process.h"

uint64_t mmu_vaddr_pagefault;

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

// get the level page table entry
pte123_t* get_pagetableentry(pte123_t *pgd, address_t *vaddr, int level, int allocate) {
  int vpns[4] = {
      vaddr->vpn1,
      vaddr->vpn2,
      vaddr->vpn3,
      vaddr->vpn4,
  };
  assert(pgd != NULL);
  assert(sizeof(pte123_t) == sizeof(pte4_t));
  assert(1 <= level && level <= 4);

  int tab_level = 1;
  pte123_t *tab = pgd;
  pte123_t *pte = NULL;

  while (tab_level <= level) {
    int vpn = vpns[tab_level - 1];
    pte = &tab[vpn];

    // move to next level
    if (tab_level < 4) {
      if (pte->present != 1) {
        if (allocate == 1) {
          // allocate a new page for next level
          pte123_t *new_tab = (pte123_t *)(&heap[(int)KERNEL_malloc(
              PAGE_TABLE_ENTRY_NUM * sizeof(pte123_t))]);

          // .paddr field is 50 bits
          tab[vpn].paddr = (uint64_t)new_tab;
          tab[vpn].present = 1;
        } else {
          return NULL;
        }
      }
      tab = (pte123_t *)((uint64_t)pte->paddr);
    }
    tab_level += 1;
  }
  return pte;
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
  assert(page_map[ppn].dirty == 0);
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

// fix pagefault : interrupt.c/pagefault_handler() call
void fix_pagefault() {
  // get page table directory from rsp
  pcb_t *pcb = get_current_pcb();
  pte123_t *pgd = pcb->mm.pgd;

  // get the faulting address from MMU register
  address_t vaddr = {.address_value = mmu_vaddr_pagefault}; // a global mmu vaddr

  // get the level 4 page table entry
  pte4_t *pte = (pte4_t *)get_pagetableentry(pgd, &vaddr, 4, 1);

  // 1 try to request one free physical page from DRAM
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE;++i) {
    if (page_map[i].allocated == 0) {
      // found i as free ppn
      map_pte4(pte, i);
      printf("\033[34;1m\tPageFault: vaddr[%lx] use free ppn %ld\033[0m\n",
             vaddr.address_value, pte->ppn);
      return;
    }
  }

  // 2. no free physical page: select one clean page (LRU) and overwrite
  // int this case, there is no DRAM - DISK transaction
  // you know you can optimize this loop
  int lru_ppn = -1;
  int lru_time = -1;
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE;++i) {
    if (page_map[i].dirty == 0 && lru_time < page_map[i].time) {
      lru_time = page_map[i].time;
      lru_ppn = i;
    }
  }
  // this si the selected ppn for vaddr
  if (-1 != lru_ppn && lru_ppn < MAX_NUM_PHYSICAL_PAGE) {
    unmap_pte4(lru_ppn);
    // load page from disk to physical memory
    // at the victim's ppn
    swap_in(pte->saddr, lru_ppn);
    map_pte4(pte, lru_ppn);

    printf("\033[34;1m\tPageFault: discard clean ppn %d as victim\033[0m\n", lru_ppn);
    return;
  }

  // 3. no free nor clean physical page: select one LRU victim
  // write back (swap out) the DIRTY victim to disk
  lru_ppn = -1;
  lru_time = -1;
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE; ++i) {
    if (lru_time < page_map[i].time) {
      lru_time = page_map[i].time;
      lru_ppn = i;
    }
  }
  assert(0 <= lru_ppn && lru_ppn < MAX_NUM_PHYSICAL_PAGE);

  // write back
  swap_out(page_map[lru_ppn].saddr, lru_ppn);
  // unmap victim
  unmap_pte4(lru_ppn);
  // load page from disk to physical memory
  swap_in(pte->saddr, lru_ppn);
  map_pte4(pte, lru_ppn);

  printf("\033[34;1m\tPageFault: write back & use ppn %d\033[0m\n", lru_ppn);
}

int allocate_physicalframe(pte4_t *pte) {
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE;++i) {
    if (page_map[i].allocated == 0) {
      // found i as frees page
      map_pte4(pte, i);
      return 1;
    }
  }
  return 0;
}

// copy one physical frame for new process to use
// And this new frame should have exactly the same data as parent process
int copy_physicalframe(pte4_t *child_pte, uint64_t parent_ppn) {
  assert(0 <= parent_ppn && parent_ppn < MAX_NUM_PHYSICAL_PAGE);
  if (allocate_physicalframe(child_pte) == 1) {
    uint64_t ppn = child_pte->ppn;
    memcpy(&pm[ppn << PHYSICAL_PAGE_OFFSET_LENGTH],
           &pm[parent_ppn << PHYSICAL_PAGE_OFFSET_LENGTH], PAGE_SIZE);
  }
  // TODO && FIXME : parent_ppn my be evicted
  // So we should fail the fork

  return 0;
}

int enough_frame(int request_num) {
  int free_num = 0;
  for (int i = 0;i < MAX_NUM_PHYSICAL_PAGE;++i) {
    if (page_map[i].allocated == 0) {
      free_num += 1;
    }
  }
  if (request_num <= free_num) {
    return 1;
  } else {
    return 0;
  }
}
