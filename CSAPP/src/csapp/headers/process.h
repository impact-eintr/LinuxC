#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>

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
  uint64_t vma_start;
  uint64_t vma_end;
  struct {
    uint64_t read : 1;
    uint64_t write : 1;
    uint64_t execute : 1;
    uint64_t private : 1;
  } vma_mode;
  char filepath[128];
} vm_area_struct;

typedef struct PROCESS_CONTROL_BLOCK_STRUCT {
  uint64_t pid;

  struct {
    // page global directory
    // This value is what's in CR3 register right now
    union {
      // page global directory
      // This value is what's in CR3 register right now
      union {
        uint64_t pgd_paddr;
        pte123_t *pgd;
      };
      // TODO vm area
    };
  } mm;

  kstack_t *kstack;

  // it's easier to store the context to PCB
  context_t context;

  struct PROCESS_CONTROL_BLOCK_STRUCT *next;
  struct PROCESS_CONTROL_BLOCK_STRUCT *prev;
} pcb_t;

void syscall_init();

pcb_t *get_current_pcb();

#endif // PROCESS_H_
