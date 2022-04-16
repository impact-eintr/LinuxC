#include "../../headers/cpu.h"
#include "../../headers/memory.h"
#include "../../headers/instruction.h"
#include "../../headers/common.h"

static inline void increase_pc() {
  cpu_pc.rip = cpu_pc.rip + sizeof(char) * MAX_INSTRUCTION_CHAR;
}

void mov_handler(od_t *src_od, od_t *dst_od) {
  if (src_od->type == OD_REG && dst_od->type == OD_REG) {
    *(uint64_t*)(dst_od->value) = *(uint64_t*)(src_od->value);
    increase_pc();
    cpu_flags.__flags_value = 0;
    return;
  } else if (src_od->type == OD_REG && dst_od->type == OD_MEM) {
    uint64_t dst_pa = va2pa(dst_od->value);
    cpu_write64bits_dram(dst_pa, *(uint64_t*)(src_od->value));
  } else if (src_od->type == OD_MEM && dst_od->type == OD_REG) {

  } else if (src_od->type == OD_IMM && dst_od->type == OD_REG) {

  }
}

void push_handler(od_t *src_od, od_t *dst_od) {}

void pop_handler(od_t *src_od, od_t *dst_od) {}

void leave_handler(od_t *src_od, od_t *dst_od) {}

void call_handler(od_t *src_od, od_t *dst_od) {}

void ret_handler(od_t *src_od, od_t *dst_od) {}

void add_handler(od_t *src_od, od_t *dst_od) {}

void sub_handler(od_t *src_od, od_t *dst_od) {}

void cmp_handler(od_t *src_od, od_t *dst_od) {}

void jne_handler(od_t *src_od, od_t *dst_od) {}

void jmp_handler(od_t *src_od, od_t *dst_od) {}

void lea_handler(od_t *src_od, od_t *dst_od) {}

void int_handler(od_t *src_od, od_t *dst_od) {}

// from inst.c
void parse_instruction(char *inst_str, inst_t *inst);

void instruction_cycle() {}
