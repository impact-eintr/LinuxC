#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

#define KERNEL_malloc malloc

typedef struct STRUCT_TRAPFRAME{
  // TODO
} trapframe_t;;


typedef struct STRUCT_USERFRAME {
  // TODO
} userframe_t;

void idt_init();

void interrupt_stack_switching(uint64_t int_vec);
void interrupt_return_stack_switching();

uint64_t get_kstack_top_TSS();
uint64_t get_kstack_RSP();


#endif // INTERRUPT_H_
