#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>

#include "cpu.h"
#include "allocator.h"

//#define KERNEL_malloc malloc
#define KERNEL_malloc(size) &heap[mem_alloc(size)]
#define KERNEL_algined_alloc mem_aligned_alloc
#define KERNEL_free(vaddr) mem_free(vaddr)

typedef struct STRUCT_TRAPFRAME{
  // error code
  uint64_t rip;
  // CS: counter segment
  // eflags
  uint64_t rsp;
  // SS: stacl segment
} trapframe_t;;


typedef struct STRUCT_USERFRAME {
  cpu_reg_t regs;
  cpu_flags_t flags;
} userframe_t;

void idt_init();

void interrupt_stack_switching(uint64_t int_vec);
void interrupt_return_stack_switching();

uint64_t get_kstack_top_TSS();
uint64_t get_kstack_RSP();

extern jmp_buf USER_INSTRUCTION_ON_IRET;

#endif // INTERRUPT_H_
