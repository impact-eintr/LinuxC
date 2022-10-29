#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>

#include "algorithm.h"
#include "cpu.h"
#include "memory.h"

#define KERNEL_STACK_SIZE (8192) // double page size

typedef union KERNEL_STACK_STRUCT {
  uint8_t stack[KERNEL_STACK_SIZE];
  struct {
    struct PROCESS_CONTROL_BLOCK_STRUCT *pcb;
  } threadinfo;
} kstack_t;

typedef struct STRUCT_PROCESS_CONTEXT {
  cpu_reg_t regs;
  cpu_flags_t flags;
} context_t;

typedef struct VIRTUAL_MEMORY_AREA_STRUCT {
  struct VIRTUAL_MEMORY_AREA_STRUCT *prev;
  struct VIRTUAL_MEMORY_AREA_STRUCT *next;
  struct VIRTUAL_MEMORY_AREA_STRUCT *anonymous_prev; // 匿名页指针
  struct VIRTUAL_MEMORY_AREA_STRUCT *anonymous_next; // 匿名页指针
  uint64_t vma_start; // 第一个属于该vma的地址
  uint64_t vma_end; // 第一个不属于该vma的地址
  union {
    uint64_t mode_value;
    struct {
      uint64_t read : 1;
      uint64_t write : 1;
      uint64_t execute : 1;
      uint64_t private : 1;
    } vma_mode;
  };
  char filepath[128];
  // used for RBT indexing
  int rbt_color;
} vm_area_t;

typedef struct PROCESS_CONTROL_BLOCK_STRUCT {
  uint64_t pid;

  struct {
    // page global directory
    // This value is what's in CR3 register right now
    // page global directory
    // This value is what's in CR3 register right now
    union {
      uint64_t pgd_paddr;
      pte123_t *pgd;
    };
    // virtual memory area
    linkedlist_internal_t vma;
  } mm;

  kstack_t *kstack;

  // it's easier to store the context to PCB
  context_t context;

  struct PROCESS_CONTROL_BLOCK_STRUCT *next;
  struct PROCESS_CONTROL_BLOCK_STRUCT *prev;
} pcb_t;

void syscall_init();

pcb_t *get_current_pcb();

void setup_pagetable_from_vma(pcb_t *proc);
int vma_add_area(pcb_t *proc, vm_area_t *area);

#endif // PROCESS_H_
