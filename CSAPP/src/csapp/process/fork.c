#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/cpu.h"
#include "../headers/interrupt.h"
#include "../headers/memory.h"
#include "../headers/syscall.h"
#include "../headers/process.h"

// from page fault
int copy_userframe(pte4_t *child_pte, uint64_t parent_ppn);
int enough_frame(int request_num);

static uint64_t fork_naive_copy();
static uint64_t fork_cow();

uint64_t syscall_fork() {
  return fork_naive_copy();
}

// Implementaion
static uint64_t get_newpid() {}

// Update rax register in user frame
void update_userframe_returnvalue(pcb_t *p, uint64_t retval) {}

static pte123_t *copy_pagetable(pte123_t *src, int level) {}

static int get_childframe_num(pte123_t *p, int level) {}

#define PAGE_TABLE_ENTRY_PADDR_MASK (~((0xffffffffffffffff >> 12) << 12))

static int compare_pagetables(pte123_t *p, pte123_t *q, int level) {}

static uint64_t fork_naive_copy() {}

static uint64_t fork_cow() { return 0; }
