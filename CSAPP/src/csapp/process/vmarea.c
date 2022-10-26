#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../headers/memory.h"
#include "../headers/interrupt.h"
#include "../headers/process.h"
#include "../headers/algorithm.h"
#include "../headers/address.h"

int allocate_physicalframe(pte4_t *pte);

// the implement of VMA list interface
static uint64_t construct_vma_node() {
  vm_area_t *vma = (vm_area_t *)KERNEL_malloc(sizeof(vm_area_t));
  vma->filepath[0] = '\0';
  vma->next = vma;
  vma->prev = vma;
  vma->vma_start = 0;
  vma->vma_end = 0;
  vma->mode_value = 0;

  return (uint64_t)vma;
}

static int destruct_vma_node(uint64_t vma_addr) {

}
