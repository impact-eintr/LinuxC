#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../../headers/interrupt.h"


typedef void (*interrupt_handler_t)();

// the entry of IDT/IVT
typedef struct IDT_ENTRY_STRUCT
{
  interrupt_handler_t handler;
} idt_entry_t;

// Interrupt Descriptor/Vector Table
idt_entry_t idt[256];

// handlers of IDT
void pagefault_handler(); // trap gate - exception
void syscall_handler();   // trap get - software interrupt / trap
void timer_handler();     // interrupt gate - local APIC

// implementation of hanlers
void do_syscall(int syscall_no);
void fix_pagefault();
void os_schedule();

// initialize od IDT
void idt_init() {
  idt[0x0e].handler = pagefault_handler;
  idt[0x80].handler = syscall_handler;
  idt[0x81].handler = timer_handler;
}

static uint64_t hardware_push_trapframe(trapframe_t *tf) {return 0;}
static void hardware_pop_trapframe() {}
static void software_push_userframe() {}
static void software_pop_userframe() {}

void interrupt_stack_switching(uint64_t int_vec) {
  assert(0 <= int_vec && int_vec <= 255);
  // TODO
  interrupt_handler_t handler = idt[int_vec].handler;
  // TODO
  handler();
  // TODO
}

void interrupt_return_stack_switching() {
  // TODO
}

// interrupt handlers

void timer_handler() {
  printf("\033[32;1mTimer interrupt to invoke OS scheduling\033[0m\n");
  software_push_userframe();
  os_schedule();
  software_pop_userframe();
}

void pagefault_handler() {
  printf("\033[32;1mPage fault handling\033[0m\n");
  software_push_userframe();
  fix_pagefault();
  os_schedule();
  software_pop_userframe();
}

void syscall_handler() {
  // TODO
}

static void print_lstack() {}
