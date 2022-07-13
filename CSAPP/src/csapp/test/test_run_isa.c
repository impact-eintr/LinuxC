#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/algorithm.h"
#include "../headers/common.h"
#include "../headers/cpu.h"
#include "../headers/instruction.h"
#include "../headers/interrupt.h"
#include "../headers/memory.h"
#include "../headers/process.h"

void page_map_init();

static void print_register() {
  printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n", cpu_reg.rax,
         cpu_reg.rbx, cpu_reg.rcx, cpu_reg.rdx);
  printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n", cpu_reg.rsi,
         cpu_reg.rdi, cpu_reg.rbp, cpu_reg.rsp);
  printf("rip = %16lx\n", cpu_pc.rip);
  printf("CF = %u\tZF = %u\tSF = %u\tOF = %u\n", cpu_flags.CF, cpu_flags.ZF,
         cpu_flags.SF, cpu_flags.OF);
}

static void print_stack() {
  int n = 10;
  uint64_t *high = (uint64_t *)&pm[va2pa(cpu_reg.rsp)];
  high = &high[n];
  uint64_t va = cpu_reg.rsp + n * 8;

  for (int i = 0; i < 2 * n; ++i) {
    uint64_t *ptr = (uint64_t *)(high - i);
    printf("0x%16lx : %16lx", va, (uint64_t)*ptr);

    if (i == n) {
      printf(" <== rsp");
    }
    printf("\n");
    va -= 8;
  }
}

pthread_once_t once;

static void TestSyscallPrintHelloWorld() {
  cpu_reg.rsp = 0x7ffffffee0f0; // init state
  char assembly[12][MAX_INSTRUCTION_CHAR] = {
      // hello world\n\0
      "movq $0x000000000a646c72, %rbx",
      "pushq %rbx",
      "movq $0x6f77206f6c6c6568, %rbx",
      "pushq %rbx",
      "movq $1, %rax",
      "movq $1, %rdi",
      "movq %rsp, %rsi",
      "movq $13, %rdx",
      "int $0x80", // syscall : write
      "movq $60, %rax",
      "movq $0, %rdi",
      "int $0x80" // syscall : exit
  };

  pthread_once(&once, page_map_init);

  // copy to physical memory
  for (int i = 0; i < 12; ++i) {
    cpu_writeinst_dram(va2pa(i * 0x40 + 0x00400000), assembly[i]);
  }
  cpu_pc.rip = 0x00400000;

  // prepare a kernel stack
  uint8_t kstack_buf[8192 * 2];
  uint64_t k_temp = (uint64_t)&kstack_buf[8192];

  kstack_t *kstack = (kstack_t *)(((k_temp >> 13) << 13) + KERNEL_STACK_SIZE);
  tss_s0_t tss;
  tss.ESP0 = (uint64_t)kstack + KERNEL_STACK_SIZE;
}

int main() { TestSyscallPrintHelloWorld(); }
