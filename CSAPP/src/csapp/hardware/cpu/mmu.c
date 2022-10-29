#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory Management Unit
#include "../../headers/address.h"
#include "../../headers/common.h"
#include "../../headers/cpu.h"
#include "../../headers/interrupt.h"
#include "../../headers/memory.h"
#include "../../headers/color.h"

// ----------------------------- //
//  Translation Lookaside Buffer
// ----------------------------- //

#define NUM_TLB_CACHE_LINE_PER_SET (8)
#ifndef USE_PAGETABLE_VA2PA
#define USE_PAGETABLE_VA2PA
#endif

cpu_cr_t cpu_controls;

typedef struct {
  int valid;
  uint64_t tag;
  uint64_t ppn;
} tlb_cacheline_t;

typedef struct {
  tlb_cacheline_t lines[NUM_TLB_CACHE_LINE_PER_SET];
} tlb_cacheset_t;

typedef struct {
  tlb_cacheset_t sets[(1 << TLB_CACHE_INDEX_LENGTH)];
  uint64_t pgd;
} tlb_cache_t;

static tlb_cache_t mmu_tlb;

static uint64_t page_walk(uint64_t vaddr_value, int write_request);
static void page_fault_handler(pte4_t *pte, address_t vaddr);

static int read_tlb(uint64_t vaddr_value, uint64_t *paddr_value_ptr,
                    int *free_tlb_line_index);
static int write_tlb(uint64_t vaddr_value, uint64_t paddr_value,
                     int free_tlb_line_index);

// vaddr - the virtual address to be translated into physical address
// write_request - if this translation is caused by write request
uint64_t va2pa(uint64_t vaddr, int write_request) {
#ifdef USE_NAVIE_VA2PA
  return vaddr % PHYSICAL_MEMORY_SPACE;
#endif
  uint64_t paddr = 0;
#if defined(USE_TLB_HARDWARE) && defined(USE_PAGETABLE_VA2PA)
  int free_tlb_line_index = -1;
  int tlb_hit = read_tlb(vaddr, &paddr, &free_tlb_line_index);
  if (tlb_hit) {
    // TLB read hit
    return paddr;
  }
// TLB read miss
#endif

#ifdef USE_PAGETABLE_VA2PA
  paddr = page_walk(vaddr, write_request);
#endif

#if defined(USE_TLB_HARDWARE) && defined(USE_PAGETABLE_VA2PA)
  if (paddr != 0) {
    // TLB write
    if (write_tlb(vaddr, paddr, free_tlb_line_index) == 1) {
      return paddr;
    }
  }
#endif

  // use page table as va2pa
  return paddr;
}

#if defined(USE_TLB_HARDWARE) && defined(USE_PAGETABLE_VA2PA)
static int read_tlb(uint64_t vaddr_value, uint64_t *paddr_value_ptr,
                    int *free_tlb_line_index) {
  address_t vaddr = {.address_value = vaddr_value};

  tlb_cacheset_t *set = &mmu_tlb.sets[vaddr.tlbi];
  *free_tlb_line_index = -1;

  for (int i = 0; i < NUM_TLB_CACHE_LINE_PER_SET; ++i) {
    tlb_cacheline_t *line = &set->lines[i];

    if (line->valid == 0) {
      *free_tlb_line_index = i;
    }
    if (line->tag == vaddr.tlbt && line->valid == 1) {
      // TLB read hit
      address_t paddr = {.ppn = line->ppn, .ppo = vaddr.vpo};
      *paddr_value_ptr = paddr.paddr_value;
      return 1;
    }
  }
  // TLB read miss
  *paddr_value_ptr = 0;
  return 0;
}

static int write_tlb(uint64_t vaddr_value, uint64_t paddr_value,
                     int free_tlb_line_index) {
  address_t vaddr = {.address_value = vaddr_value};
  address_t paddr = {.address_value = paddr_value};

  tlb_cacheset_t *set = &mmu_tlb.sets[vaddr.tlbi];
  if (0 <= free_tlb_line_index &&
      free_tlb_line_index < NUM_TLB_CACHE_LINE_PER_SET) {
    tlb_cacheline_t *line = &set->lines[free_tlb_line_index];
    line->valid = 1;
    line->ppn = paddr.ppn;
    line->tag = vaddr.tlbt;
    return 1;
  }
  // no free TLB cache line, select on DRAM victim
  int random_victim_index = rand() % NUM_TLB_CACHE_LINE_PER_SET;
  tlb_cacheline_t *line = &set->lines[random_victim_index];
  line->valid = 1;
  line->ppn = paddr.ppn;
  line->tag = vaddr.tlbt;

  return 1;
}

void flush_tlb() {
  memset(&mmu_tlb, 0, sizeof(tlb_cache_t));
}

#endif

#ifdef USE_PAGETABLE_VA2PA
// input virtual address
// output physics address
static uint64_t page_walk(uint64_t vaddr_value, int write_request) {
  // parse address
  address_t vaddr = {.vaddr_value = vaddr_value};
  int vpns[4] = {
      vaddr.vpn1,
      vaddr.vpn2,
      vaddr.vpn3,
      vaddr.vpn4,
  };
  int vpo = vaddr.vpo;

  int page_table_size = PAGE_TABLE_ENTRY_NUM * sizeof(pte123_t);

  // CR3 register's value is malloced on the heap of the simuator
  pte123_t *pgd = (pte123_t *)cpu_controls.cr3;
  assert(pgd != NULL);
  assert(sizeof(pte123_t) == sizeof(pte4_t));
  assert(page_table_size == (1 << 12));

  int level = 0;
  pte123_t *tab = pgd;
  while (level < 3) {
    int vpn = vpns[level];
    if (tab[vpn].present != 1) {
      // page fault
      printf(REDSTR("MMU (%lx): level %d page fault: [%x].present ==  0\n"),
             vaddr_value, level + 1, vpn);
      goto PAISE_PAGE_FAULT;
    }
    tab = (pte123_t *)((uint64_t)tab[vpn].paddr); // move to next level
    level += 1;
  }

  pte4_t *pte = &((pte4_t *)tab)[vaddr.vpn4];
  if (pte->present == 1) {
    // find page table entry
    address_t paddr = {.ppn = pte->ppn, .ppo = vpo};

    // 当child的一个写操作要修改只读的page时，将发生一个Protection Fault(写保护Fault)
    if (pte->readonly == 1 && write_request) {
      printf(REDSTR("\tProtection Fault\n"));
      goto PAISE_PAGE_FAULT;
    }
    return paddr.paddr_value;
  } else {
    printf(REDSTR("MMU (%lx): level 4 page fault: [%x].present == 0\n"),
           vaddr_value, vaddr.vpn4);
  }
PAISE_PAGE_FAULT:
  mmu_vaddr_pagefault = vaddr.vaddr_value;
  // raise exception 14 (page fault) here
  // This interrupt will not return
  interrupt_stack_switching(0x0e);
  return 0;
}
#endif
