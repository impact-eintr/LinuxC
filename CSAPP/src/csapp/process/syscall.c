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
#include "../headers/color.h"

typedef void (*syscall_handler_t)();

// the entry of syscall table
typedef struct SYSCALL_ENTRY_STRUCT {
  syscall_handler_t handler;
} syscall_entry_t;

// table of syscall
syscall_entry_t syscall_table[64];

// handlers of syscalls
static void write_handler();
static void getpid_handler();
static void fork_handler();
static void execve_handler();
static void exit_handler();
static void wait_handler();
static void kill_handler();

static void destrory_user_registers() {
  // when run kernel thread, user registers should be useless
  memset(&cpu_reg, 0, sizeof(cpu_reg));
  memset(&cpu_flags, 0, sizeof(cpu_flags));
}

// initialize of IDT
void syscall_init() {
  syscall_table[0x01].handler = write_handler;
  syscall_table[0x27].handler = getpid_handler;
  syscall_table[0x39].handler = fork_handler;
  syscall_table[0x3b].handler = execve_handler;
  syscall_table[0x3c].handler = exit_handler;
  syscall_table[0x3d].handler = wait_handler;
  syscall_table[0x3e].handler = kill_handler;
}

static void write_handler() {
  // assemley begin
  // get user thread parameters
  uint64_t file_no = cpu_reg.rdi;
  uint64_t buf_vaddr = cpu_reg.rsi;
  uint64_t buf_length = cpu_reg.rdx;
  // assemley end

  destrory_user_registers();

  for (int i = 0; i < buf_length; ++i) {
    // printf as yellow
    uint64_t pc_pa = va2pa(buf_vaddr + i, 0);
    printf(PINKSTR("%c"), cpu_read8bits_dram(pc_pa));
  }
}

static void getpid_handler() {}

static void fork_handler() {
  uint64_t kernel_rsp = cpu_reg.rsp;
  destrory_user_registers();
  cpu_reg.rsp = kernel_rsp;

  syscall_fork();
}

static void execve_handler() {}

static void exit_handler() {
  uint64_t exit_status = cpu_reg.rdi;
  printf(REDSTR("Exit with %lx\n"), exit_status);
}

static void wait_handler() {}

static void kill_handler() {}


void do_syscall(int syscall_no) {
  assert(0 <= syscall_no && syscall_no <= 63);
  syscall_table[syscall_no].handler();
}
