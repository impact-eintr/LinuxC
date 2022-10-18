#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  // return value
  union {
    struct {
      uint8_t al;
      uint8_t ah;
    };
    uint16_t ax;
    uint32_t eax;
    uint64_t rax;
  };

  // callee saved
  union {
    struct {
      uint8_t bl;
      uint8_t bh;
    };
    uint16_t bx;
    uint32_t ebx;
    uint64_t rbx;
  };

  // 4th argument
  union {
    struct {
      uint8_t cl;
      uint8_t ch;
    };
    uint16_t cx;
    uint32_t ecx;
    uint64_t rcx;
  };

  // 3rd argument
  union {
    struct {
      uint8_t dl;
      uint8_t dh;
    };
    uint16_t dx;
    uint32_t edx;
    uint64_t rdx;
  };

  // 2nd argument
  union {
    struct {
      uint8_t sil;
      uint8_t sih;
    };
    uint16_t si;
    uint32_t esi;
    uint64_t rsi;
  };

  // 1st argument
  union {
    struct {
      uint8_t dil;
      uint8_t dih;
    };
    uint16_t di;
    uint32_t edi;
    uint64_t rdi;
  };

  // callee saved frame pointer
  union {
    struct {
      uint8_t bpl;
      uint8_t bph;
    };
    uint16_t bp;
    uint32_t ebp;
    uint64_t rbp;
  };

  // stack pointer
  union {
    struct {
      uint8_t spl;
      uint8_t sph;
    };
    uint16_t sp;
    uint32_t esp;
    uint64_t rsp;
  };

  // 5th argument
  union {
    uint64_t r8;
    uint32_t r8d;
    uint16_t r8w;
    uint8_t r8b;
  };
  // 6th argument
  union {
    uint64_t r9;
    uint32_t r9d;
    uint16_t r9w;
    uint8_t r9b;
  };
  // caller saved
  union {
    uint64_t r10;
    uint32_t r10d;
    uint16_t r10w;
    uint8_t r10b;
  };
  // caller saved
  union {
    uint64_t r11;
    uint32_t r11d;
    uint16_t r11w;
    uint8_t r11b;
  };

  // callee saved
  union {
    uint64_t r12;
    uint32_t r12d;
    uint16_t r12w;
    uint8_t r12b;
  };
  // callee saved
  union {
    uint64_t r13;
    uint32_t r13d;
    uint16_t r13w;
    uint8_t r13b;
  };
  // callee saved
  union {
    uint64_t r14;
    uint32_t r14d;
    uint16_t r14w;
    uint8_t r14b;
  };
  // callee saved
  union {
    uint64_t r15;
    uint32_t r15d;
    uint16_t r15w;
    uint8_t r15b;
  };
} cpu_reg_t;
extern cpu_reg_t cpu_reg;


/*=============================*/
/*           cpu core          */
/*=============================*/

// the 4 flags be a uint64_t in total
typedef union {
  uint64_t __flags_value;
  struct {
    uint16_t CF;
    uint16_t ZF;
    uint16_t SF;
    uint16_t OF;
  };
} cpu_flags_t;
extern cpu_flags_t cpu_flags;

// program counter or instruction pointer
typedef union {
  uint64_t rip;
  uint32_t eip;
} cpu_pc_t;
extern cpu_pc_t cpu_pc;


// TSS
// we only use stack0 of TSS
// This information is stored in main memory
typedef struct TSS_S0 {
  uint64_t ESP0;
  uint64_t SS0;
} tss_s0_t;
// TSS is store in DRAM
// Intel thinks that each process can have ite own TSS
// But we can use only one TSS globally
// pointing to task-State Segment (in main memory) of the current process
extern tss_s0_t tr_global_tss;

// Control registers
typedef struct {
  uint64_t cr0;
  uint64_t cr1;
  uint64_t cr2;
  uint64_t cr3; // should be a 40-bits ppN for PGD in DRAM
                // but we are using 48-bits virtual address on simulator's heap
} cpu_cr_t;
extern cpu_cr_t cpu_controls;


#define NUM_INSTRTYPE 14
void instruction_cycle();

// global mmu pagefault virtual address
extern uint64_t mmu_vaddr_pagefault;

void flush_tlb();

uint64_t va2pa(uint64_t vaddr, int write_request);

#endif
