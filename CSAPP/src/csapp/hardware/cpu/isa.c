#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../headers/cpu.h"
#include "../../headers/memory.h"
#include "../../headers/instruction.h"
#include "../../headers/algorithm.h"
#include "../../headers/interrupt.h"
#include "../../headers/common.h"

static inline void increase_pc() {
  cpu_pc.rip = cpu_pc.rip + sizeof(char) * MAX_INSTRUCTION_CHAR;
}

void mov_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_REG && dst_od->type == OD_REG) { // mov %rax,%rdi
    *(uint64_t*)(dst_od->value) = *(uint64_t*)(src_od->value);
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  } else if (src_od->type == OD_REG && dst_od->type == OD_MEM) { // mov %rax,(%rdi)
    uint64_t dst_pa = va2pa(dst_od->value);
    cpu_write64bits_dram(dst_pa, *(uint64_t*)(src_od->value));
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  } else if (src_od->type == OD_MEM && dst_od->type == OD_REG) { // mov (%rax),%rdi
    uint64_t src_pa = va2pa(src_od->value);
    *(uint64_t*)(dst_od->value) = cpu_read64bits_dram(src_pa);
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  } else if (src_od->type == OD_IMM && dst_od->type == OD_REG) { // mov $0x1234,%rax
    *(uint64_t*)(dst_od->value) = (src_od->value);
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  } else if (src_od->type == OD_IMM && dst_od->type == OD_MEM) { // mov $0x1234,(%rax)
    uint64_t dst_pa = va2pa(src_od->value);
    cpu_write64bits_dram(dst_pa, src_od->value);
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  }
}

void push_handler(od_t *src_od, od_t *dst_od) { // src:reg dst:empty
  if (src_od->type == OD_REG) {
    uint64_t rsp_pa = va2pa(cpu_reg.rsp - 8);
    cpu_write64bits_dram(rsp_pa, *(uint64_t *)(src_od->value));

    cpu_reg.rsp = cpu_reg.rsp - 8;
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  }
}

void pop_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_REG) {
    uint64_t rsp_pa = va2pa(cpu_reg.rsp);
    uint64_t old_value = cpu_read64bits_dram(rsp_pa);
    cpu_reg.rsp = cpu_reg.rsp + 8;
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  }
}

void leave_handler(od_t *src_od, od_t *dst_od) {
  // movq %rbp, %rsp
  cpu_reg.rsp = cpu_reg.rbp;

  // popq %rbp
  uint64_t rsp_pa = va2pa(cpu_reg.rsp);
  uint64_t old_val = cpu_read64bits_dram(rsp_pa);
  cpu_reg.rsp = cpu_reg.rsp + 8;
  cpu_reg.rbp = old_val;
  increase_pc();
  cpu_flags.__flags_value = 0;
}

// NOTE Call 和 Ret 应该以递归的思想考虑
//      先移动栈顶指针 然后在新的栈顶位置写入原本要执行的指令
//      然后将指令指针移动到目标指令处
void call_handler(od_t *src_od, od_t *dst_od) {
  cpu_reg.rsp = cpu_reg.rsp - 8; // push the return value
  uint64_t rsp_pa = va2pa(cpu_reg.rsp);
  cpu_write64bits_dram(rsp_pa, cpu_pc.rip + sizeof(char) * MAX_INSTRUCTION_CHAR); // push over
  // jump to target function adress
  cpu_pc.rip = (src_od->value);
  cpu_flags.__flags_value = 0;
}

// 将之前压栈保存的指令地址取出 将指令指针移动到这里
void ret_handler(od_t *src_od, od_t *dst_od) {
  uint64_t rsp_pa = va2pa(cpu_reg.rsp);
  uint64_t ret_addr = cpu_read64bits_dram(rsp_pa);
  cpu_reg.rsp = cpu_reg.rsp + 8; // pop over
  cpu_pc.rip = ret_addr;
  cpu_flags.__flags_value = 0;
}

void add_handler(od_t *src_od, od_t *dst_od) { // add %rax, %rbx
  if (src_od->type == OD_REG && dst_od->type == OD_REG) {
    uint64_t val = *(uint64_t*)(dst_od->value) + *(uint64_t*)(src_od->value);

    int val_sign = ((val >> 63) & 0x1);
    int src_sign = ((*(uint64_t*)(src_od->value) >> 63) & 0x1);
    int dst_sign = ((*(uint64_t*)(dst_od->value) >> 63) & 0x1);

    // set condition flags
    cpu_flags.CF = (val < *(uint64_t*)(src_od->value));
    cpu_flags.ZF = (val == 0);
    cpu_flags.SF = val_sign;
    cpu_flags.OF = (src_sign == 0 && dst_sign == 0 && val_sign == 1) ||
      (src_sign == 1 && dst_sign == 1 && val_sign == 0);

    *(uint64_t*)(dst_od->value) = val;
    increase_pc();
    return;
  }
}

void sub_handler(od_t *src_od, od_t *dst_od) { // sub $0x1234, %rax
  if (src_od->type == OD_IMM && dst_od->type == OD_REG) {
    uint64_t val = *(uint64_t*)(dst_od->value) + (~(src_od->value) + 1);

    int val_sign = ((val >> 63) & 0x1);
    int src_sign = (((src_od->value) >> 63) & 0x1);
    int dst_sign = ((*(uint64_t*)(dst_od->value) >> 63) & 0x1);

    // set condition flags
    cpu_flags.CF = (val > *(uint64_t*)(dst_od->value));
    cpu_flags.ZF = (val == 0);
    cpu_flags.SF = val_sign;
    cpu_flags.OF = (src_sign == 1 && dst_sign == 0 && val_sign == 1) ||
      (src_sign == 0 && dst_sign == 1 && val_sign == 0);

    *(uint64_t*)(dst_od->value) = val;
    increase_pc();
    return;
  }
}

void cmp_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_IMM && dst_od->type == OD_MEM) {
    uint64_t dst_pa = va2pa(dst_od->value);
    uint64_t dval = cpu_read64bits_dram(dst_pa);
    uint64_t val = dval + (~(src_od->value) + 1);

    int val_sign = ((val >> 63) & 0x1);
    int src_sign = (((src_od->value) >> 63) & 0x1);
    int dst_sign = ((dval >> 63) & 0x1);

    // set condition flags
    cpu_flags.CF = (val > dval);
    cpu_flags.ZF = (val == 0);
    cpu_flags.SF = val_sign;
    cpu_flags.OF = (src_sign == 1 && dst_sign == 0 && val_sign == 1) ||
      (src_sign == 0 && dst_sign == 1 && val_sign == 0);

    increase_pc();
    return;
  }
}

void jne_handler(od_t *src_od, od_t *dst_od) {
  if (cpu_flags.ZF == 0) {
    cpu_pc.rip = (src_od->value);
  } else {
    increase_pc();
  }
  cpu_flags.__flags_value = 0;
}

void jmp_handler(od_t *src_od, od_t *dst_od) {
  cpu_pc.rip = (src_od->value);
  cpu_flags.__flags_value = 0;
}

void lea_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_MEM && dst_od->type == OD_REG) {
    // src: virtual address - The effective address computed from instruction
    // dst: register - The register to load the effective address
    *(uint64_t *)(dst_od->value) = src_od->value;
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  }
}

void int_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_IMM) {
    // src: interrupt vector

    // Be careful here. Think why we need to increase RIP before interrupt?
    // This `int` instruction is executed by process 1,
    // but interrupt will cause OS's scheduling to process 2.
    // So this `int_handler` will not return.
    // When the execution of process 1 resumed, the system call is finished.
    // We want to execute the next instruction, so RIP pushed to trap frame
    // must be the next instruction.
    increase_pc();
    cpu_flags.__flags_value = 0;

    // This function will not return.
    interrupt_stack_switching(src_od->value);
  }
}

void nop_handler(od_t *src_od, od_t *dst_od) {
  increase_pc();
  cpu_flags.__flags_value = 0;
}

// from inst.c
void parse_instruction(char *inst_str, inst_t *inst);

// tiem, the craft of god
static uint64_t global_time = 0;
static uint64_t timer_period = 5;

#define DEBUG_INSTRUCTION_CYCLE
// instruction sycle is implement in cpu
// the only exposed interface utside CPU
void instruction_cycle() {
  // this is the entry point of the re-execution of interrupt return instruction.
  // When a new process is sheduled, the first instruction/return instruction
  // should start here, jumping out of the call stack of old process,
  // This is especially useful fot page fault handling.
  setjmp(USER_INSTRUCTION_ON_IRET);

  global_time += 1;
  // FETCH: get the instruction string by program counter
  char inst_str[MAX_INSTRUCTION_CHAR + 10];
  uint64_t pc_pa = va2pa(cpu_pc.rip);
  cpu_readinst_dram(pc_pa, inst_str);

#ifdef DEBUG_INSTRUCTION_CYCLE
  printf("EXECUTE INSTRUCTION %8lx       %s\n", cpu_pc.rip, inst_str);
#endif

  // DECODE: decode the run-time instruction operands
  inst_t inst;
  parse_instruction(inst_str, &inst);

  // EXECUTE: get the funtion pointer of handler by the operator
  //          update  CPU and Memory  according the instruction
  inst.op(&(inst.src), &(inst.dst));

  // check timer interrupt from APIC
  if ((global_time % timer_period) == 0) {
    interrupt_stack_switching(0x81);
  }
}
