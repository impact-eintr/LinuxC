#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>

typedef enum OPERAND_TYPE {
  OD_EMPTY,
  OD_IMM,
  OD_REG,
  OD_MEM,
} od_type_t;

typedef struct OPERAND_STRUCT {
  od_type_t type;
  uint64_t value;
} od_t;

typedef void (*op_t)(od_t*, od_t*);

typedef struct INST_STRUCT {
  op_t op;
  od_t src;
  od_t dst;
} inst_t;

#define MAX_NUM_INSTRUCTION_CYCLE 100
#define DEREF_VALUE(od) (*(uint64_t *)(od->value))

#endif // INSTRUCTION_H_
