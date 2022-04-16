#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../headers/memory.h"

uint64_t va2pa(int64_t vaddr) {
#ifdef USE_NAVIE_VA2PA
  return vaddr % PHYSICAL_MEMORY_SPACE;
#endif
  return 0x0;
}
