#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "../../headers/cpu.h"
#include "../../headers/memory.h"
#include "../../headers/common.h"

uint64_t cpu_read64bits_dram(uint64_t paddr) {
  uint64_t val = 0x0;
#ifdef USE_SRAM_CACHE
  // TODO
#else
  // little-endian
  val += (((uint64_t)pm[paddr + 0]) << 0);
  val += (((uint64_t)pm[paddr + 1]) << 8);
  val += (((uint64_t)pm[paddr + 2]) << 16);
  val += (((uint64_t)pm[paddr + 3]) << 24);
  val += (((uint64_t)pm[paddr + 4]) << 32);
  val += (((uint64_t)pm[paddr + 5]) << 40);
  val += (((uint64_t)pm[paddr + 6]) << 48);
  val += (((uint64_t)pm[paddr + 7]) << 56);
#endif

#ifdef USE_PAGETABLE_VA2PA
  // TODO
#endif
  return val;
}

void cpu_write64bits_dram(uint64_t paddr, uint64_t data) {
#ifdef USE_SRAM_CACHE
  // TODO
#else
  pm[paddr +0] = (data >> 0) & 0xff;
  pm[paddr +1] = (data >> 8) & 0xff;
  pm[paddr +2] = (data >> 16) & 0xff;
  pm[paddr +3] = (data >> 24) & 0xff;
  pm[paddr +4] = (data >> 32) & 0xff;
  pm[paddr +5] = (data >> 40) & 0xff;
  pm[paddr +6] = (data >> 48) & 0xff;
  pm[paddr +7] = (data >> 56) & 0xff;
#endif

#ifdef USE_PAGETABLE_VA2PA
  // TODO
#endif
}

void cpu_readinst_dram(uint64_t paddr, char *buf) {
  for (int i = 0;i < MAX_INSTRUCTION_CHAR;++i) {
    buf[i] = (char)pm[paddr+i];
  }
}

void cpu_writeinst_dram(uint64_t paddr, const char *str) {
  int len = strlen(str);
  assert(len < MAX_INSTRUCTION_CHAR);

  for (int i = 0;i < MAX_INSTRUCTION_CHAR;++i) {
    if (i < len) {
      pm[paddr + i] = (uint8_t)str[i];
    } else {
      pm[paddr + i] = 0;
    }
  }
}
