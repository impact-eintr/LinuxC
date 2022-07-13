#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/cpu.h"
#include "../headers/interrupt.h"
#include "../headers/memory.h"
#include "../headers/process.h"

pcb_t *get_current_pcb() {
  kstack_t *ks = (kstack_t *)get_kstack_RSP();
  pcb_t *current_pcb = ks->threadinfo.pcb;
  return current_pcb;
}

void os_schedule() {}
