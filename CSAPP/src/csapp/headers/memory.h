#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>

#define PHYSICAL_MEMORY_SPACE (65536)
#define MAX_NUM_PHYSICAL_PAGE (16)

uint8_t pm[PHYSICAL_MEMORY_SPACE];

// page table struct

typedef union {
  uint64_t pte_value;
} pte123_t; // 8 bytes = 64bits

typedef union {
  uint64_t pte_value;
} pte4_t; // 8 bytes = 64bits

/*======================================*/
/*      memory R/W                      */
/*======================================*/

// used by instructions: read or write uint64_t to DRAM
uint64_t cpu_read64bits_dram(uint64_t paddr);
void cpu_write64bits_dram(uint64_t paddr, uint64_t data);
void cpu_readinst_dram(uint64_t paddr, char *buf);
void cpu_writeinst_dram(uint64_t paddr, const char *str);

void bus_read_cacheline(uint64_t paddr, uint8_t *block);
void bus_write_cacheline(uint64_t paddr, uint8_t *block);
#endif // MEMORY_H_
