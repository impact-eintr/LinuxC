#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/cpu.h"
#include "../headers/interrupt.h"
#include "../headers/memory.h"
#include "../headers/process.h"
#include "../headers/color.h"

pcb_t *get_current_pcb() {
  kstack_t *ks = (kstack_t *)get_kstack_RSP();
  pcb_t *current_pcb = ks->threadinfo.pcb;
  return current_pcb;
}

static void store_context(pcb_t *proc) {
  context_t ctx = {
    .regs = cpu_reg,
    .flags = cpu_flags
  };
  memcpy(&(proc->context), &ctx, sizeof(context_t));
}

static void restore_context(pcb_t *proc) {
  memcpy(&cpu_reg, &(proc->context.regs), sizeof(cpu_reg_t));
  memcpy(&cpu_flags, &(proc->context.flags), sizeof(cpu_flags_t));
}

void os_schedule() {
  pcb_t *pcb_old = get_current_pcb();
  pcb_t *pcb_new = pcb_old->next;
  printf(REDSTR("\tOS schedule [%ld] -> [%ld]\n"), pcb_old->pid, pcb_new->pid);

  store_context(pcb_old);

  restore_context(pcb_new);

  tr_global_tss.ESP0 = get_kstack_RSP() + KERNEL_STACK_SIZE;
  cpu_controls.cr3 = (uint64_t)(pcb_new->mm.pgd);
  // flush mmu and tlb
#if defined(USE_TLB_HARDWARE) && defined(USE_PAGETABLE_VA2PA)
  if (pcb_old->mm.pgd != pcb_new->mm.pgd) {
    flush_tlb();
  }
#endif
}
