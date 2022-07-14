#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/algorithm.h"
#include "../headers/common.h"
#include "../headers/cpu.h"
#include "../headers/memory.h"
#include "../headers/instruction.h"
#include "../headers/interrupt.h"
#include "../headers/address.h"
#include "../headers/memory.h"
#include "../headers/process.h"

void map_pte4(pte4_t *pte, uint64_t ppn);
void page_map_init();

static void link_page_table(pte123_t *pgd,pte123_t *pud,pte123_t *pmd,pte4_t *pt,
  int ppn, address_t *vaddr) {

  int vpn1 = vaddr->vpn1;
  int vpn2 = vaddr->vpn2;
  int vpn3 = vaddr->vpn3;
  int vpn4 = vaddr->vpn4;

  (&(pgd[vpn1]))->paddr = (uint64_t)&pud[0];
  (&(pgd)[vpn1])->present = 1;

  (&(pud[vpn2]))->paddr = (uint64_t)&pmd[0];
  (&(pud)[vpn2])->present = 1;

  (&(pmd[vpn3]))->paddr = (uint64_t)&pt[0];
  (&(pmd)[vpn3])->present = 1;

  (&(pt[vpn4]))->ppn = ppn;
  (&(pt)[vpn4])->present = 1;

  map_pte4(&pt[vpn4], ppn);
}

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

static void TestSyscallPrintHelloWorld() {
  // init state
  cpu_pc.rip = 0x00400000;
  cpu_reg.rsp = 0x7ffffffee0f0;

  address_t code_addr = {.address_value = cpu_pc.rip};

  page_map_init();

  // pcb is need to trigger page fault
  pcb_t p1;
  memset(&p1, 0, sizeof(pcb_t));
  p1.pid = 1;
  p1.next = &p1;
  p1.prev = &p1;

  // prepare PGD
  pte123_t p1_pgd[512];
  memset(&p1_pgd, 0, sizeof(pte123_t) * 512);
  p1.mm.pgd = &p1_pgd[0];
  // prepare PUD
  pte123_t p1_pud[512];
  memset(&p1_pud, 0, sizeof(pte123_t) * 512);
  // prepare PMD
  pte123_t p1_pmd[512];
  memset(&p1_pmd, 0, sizeof(pte123_t) * 512);
  // prepare PT
  pte4_t p1_pt[512];
  memset(&p1_pt, 0, sizeof(pte4_t) * 512);
  link_page_table(&p1_pgd[0], &p1_pud[0], &p1_pmd[0], &p1_pt[0], 0, &code_addr);

  // Mark all other page_map as allocated
  uint64_t free_ppn = 13;
  pte4_t other_process_pte4[MAX_NUM_PHYSICAL_PAGE];
  for (int i = 1; i < MAX_NUM_PHYSICAL_PAGE; ++i) {
    if (i != free_ppn) {
      map_pte4(&other_process_pte4[i], i);
    }
  }

  // create kernel stacks for trap into kernel
  uint8_t stack_buf[8192 * 2];
  uint64_t p1_stack_bottom = (((uint64_t)&stack_buf[8192]) >> 13) << 13;
  p1.kstack = (kstack_t *)p1_stack_bottom;
  p1.kstack->threadinfo.pcb = &p1;

  // run p1
  tr_global_tss.ESP0 = p1_stack_bottom + KERNEL_STACK_SIZE;

  cpu_controls.cr3 = p1.mm.pgd_paddr;
  idt_init();
  syscall_init();

  // load code to frame 0
  char code[14][MAX_INSTRUCTION_CHAR] = {
      // hello world hello world\n
      "movq $0x0a646c726f77206f, %rbx",
      "pushq %rbx",
      "movq $0x6c6c656820646c72, %rbx",
      "pushq %rbx",
      "movq $0x6f77206f6c6c6568, %rbx",
      "pushq %rbx",
      "movq $1, %rax",
      "movq $1, %rdi",
      "movq %rsp, %rsi",
      "movq $25, %rdx",
      "int $0x80", // syscall : write
      "movq $60, %rax",
      "movq $0, %rdi",
      "int $0x80", // syscall : exit
  };

  memcpy((char *)(&pm[0 + code_addr.ppo]), &code, sizeof(char) * 14 * MAX_INSTRUCTION_CHAR);

  // this should trigger page fault
  for (int i = 0; i < 14; ++i) {
    instruction_cycle();
  }
  printf("\033[32;1m\tPass\033[0m\n");
}

static void TestAddFunctionCallAndComputation() {
  // init state
  cpu_pc.rip = 0x00400000;
  cpu_reg.rax = 0xabcd;
  cpu_reg.rbx = 0x8000670;
  cpu_reg.rcx = 0x8000670;
  cpu_reg.rdx = 0x12340000;
  cpu_reg.rsi = 0x7ffffffee208;
  cpu_reg.rdi = 0x1;
  cpu_reg.rbp = 0x7ffffffee110;
  cpu_reg.rsp = 0x7ffffffee0f0;

  address_t code_addr = {.address_value = cpu_pc.rip};

  page_map_init();

  // pcb is need to trigger page fault
  pcb_t p1;
  memset(&p1, 0, sizeof(pcb_t));
  p1.pid = 1;
  p1.next = &p1;
  p1.prev = &p1;

  // prepare PGD
  pte123_t p1_pgd[512];
  memset(&p1_pgd, 0, sizeof(pte123_t) * 512);
  p1.mm.pgd = &p1_pgd[0];
  // prepare PUD
  pte123_t p1_pud[512];
  memset(&p1_pud, 0, sizeof(pte123_t) * 512);
  // prepare PMD
  pte123_t p1_pmd[512];
  memset(&p1_pmd, 0, sizeof(pte123_t) * 512);
  // prepare PT
  pte4_t p1_pt[512];
  memset(&p1_pt, 0, sizeof(pte4_t) * 512);
  link_page_table(&p1_pgd[0], &p1_pud[0], &p1_pmd[0], &p1_pt[0], 0, &code_addr);
  // Mark all other page_map as allocated
  uint64_t free_ppn = 13;
  pte4_t other_process_pte4[MAX_NUM_PHYSICAL_PAGE];
  for (int i = 1; i < MAX_NUM_PHYSICAL_PAGE; ++i) {
    if (i != free_ppn) {
      map_pte4(&other_process_pte4[i], i);
    }
  }

  // create kernel stacks for trap into kernel
  uint8_t stack_buf[8192 * 2];
  uint64_t p1_stack_bottom = (((uint64_t)&stack_buf[8192]) >> 13) << 13;
  p1.kstack = (kstack_t *)p1_stack_bottom;
  p1.kstack->threadinfo.pcb = &p1;

  // run p1
  tr_global_tss.ESP0 = p1_stack_bottom + KERNEL_STACK_SIZE;

  cpu_controls.cr3 = p1.mm.pgd_paddr;
  idt_init();
  syscall_init();

  // TODO use mov implement this function

  // load code to frame 0
  char assembly[25][MAX_INSTRUCTION_CHAR] = {
      "push   %rbp",
      "mov    $0x7ffffffee110, %rbp", // 0
      "mov    $0x0000000000000000,(%rbp)", // 0
      "mov    $0x7ffffffee110, %rbp", // 0
      "mov    $0x0000000000000000,(%rbp)", // 0
      "mov    $0x7ffffffee110, %rbp", // 0
      "mov    $0x0000000000000000,(%rbp)", // 0
      "mov    $0x7ffffffee110, %rbp", // 0
      "mov    $0x0000000000000000,(%rbp)", // 0
      "mov    $0x7ffffffee110, %rbp", // 0
      "mov    $0x0000000000000000,(%rbp)", // 0
      "mov    $0x0000000000000000,-0x8(%rbp)",       // 0
      "mov    $0x0000000012340000,-0x16(%rbp)",      // 0
      "mov    $0x000000000000abcd,%rbp",             // 0
      "mov    $0x0000000000000000,%rsp",             // 0
      "push   %rbp",                                 // 0
      "mov    %rsp,%rbp",                            // 1
      "mov    %rdi,-0x18(%rbp)",                     // 2
      "mov    %rsi,-0x20(%rbp)",                     // 3
      "mov    -0x18(%rbp),%rdx",                     // 4
      "mov    -0x20(%rbp),%rax",                     // 5
      "add    %rdx,%rax",                            // 6
      "mov    %rax,-0x8(%rbp)",                      // 7
      "mov    -0x8(%rbp),%rax",                      // 8
      "pop    %rbp",                                 // 9
      "retq",                                        // 10
      "mov    %rdx,%rsi",                            // 11
      "mov    %rax,%rdi",                            // 12
      "callq  0x00400000",                           // 13
      "mov    %rax,-0x8(%rbp)",                      // 14
  };
  memcpy((char *)(&pm[0 + code_addr.ppo]), &assembly,
         sizeof(char) * 20 * MAX_INSTRUCTION_CHAR);

  //cpu_write64bits_dram(va2pa(0x7ffffffee110), 0x0000000000000000); // rbp
  //cpu_write64bits_dram(va2pa(0x7ffffffee108), 0x0000000000000000);
  //cpu_write64bits_dram(va2pa(0x7ffffffee100), 0x0000000012340000);
  //cpu_write64bits_dram(va2pa(0x7ffffffee0f8), 0x000000000000abcd);
  //cpu_write64bits_dram(va2pa(0x7ffffffee0f0), 0x0000000000000000); // rsp

  // this should trigger page fault
  for (int i = 0; i < 20; ++i) {
    instruction_cycle();
  }
  printf("\033[32;1m\tPass\033[0m\n");
}

int main() {
  //TestSyscallPrintHelloWorld();
  TestAddFunctionCallAndComputation();
}
