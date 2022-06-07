#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>

#include "cpu.h"
#include "memory.h"

#define LERNEL_STACK_SIZE (8192)

typedef union KERNEL_STACK_STRUCT {
  uint8_t stack[LERNEL_STACK_SIZE];
  struct {
    struct PROCESS_CONTROL_BLOCK_STRUCT *pcb;
  } threadinfo;
} kstack_t;

typedef struct STRUCT_PROCESS_CONTEXT {
  cpu_reg_t regs;
  cpu_flags_t falgs;
} context_t;

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
