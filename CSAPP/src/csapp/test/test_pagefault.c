#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/algorithm.h"
#include "../headers/address.h"
#include "../headers/common.h"
#include "../headers/cpu.h"
#include "../headers/instruction.h"
#include "../headers/interrupt.h"
#include "../headers/memory.h"
#include "../headers/process.h"

void map_pte4(pte4_t *pte, uint64_t ppn);
void unmap_pte4(uint64_t ppn);
void page_map_init();
void pagemap_dirty(uint64_t ppn);
void pagemap_update_time(uint64_t ppn);
void set_pagemap_swapaddr(uint64_t ppn, uint64_t swap_address);
uint64_t allocate_swappage(uint64_t ppn);

static void link_page_table(pte123_t *pgd,pte123_t *pud,pte123_t *pmd,pte4_t *pt,
  int ppn, address_t *vaddr) {

  int vpn1 = vaddr->vpn1;
  int vpn2 = vaddr->vpn2;
  int vpn3 = vaddr->vpn3;
  int vpn4 = vaddr->vpn4;

  (&(pgd[vpn1]))->paddr = (uint64_t)&pud[0];
  (&(pgd)[vpn1])->present = 1;

  (&(pud[vpn2]))->paddr = (uint64_t)&pmd[0];
  (&(pud)[vpn2])->present = 1;

  (&(pmd[vpn3]))->paddr = (uint64_t)&pt[0];
  (&(pmd)[vpn3])->present = 1;

  (&(pt[vpn4]))->ppn = ppn;
  (&(pt)[vpn4])->present = 1;

  map_pte4(&pt[vpn4], ppn);
}

static void TestPageFaultHandlingCase1() {

}

int main() {
  TestPageFaultHandlingCase1();
  return 0;
}
