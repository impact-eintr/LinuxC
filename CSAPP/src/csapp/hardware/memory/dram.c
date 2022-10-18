#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../../headers/address.h"
#include "../../headers/common.h"
#include "../../headers/cpu.h"
#include "../../headers/memory.h"

#define USE_SRAM_CACHE

uint8_t pm[PHYSICAL_MEMORY_SPACE];

#ifdef USE_SRAM_CACHE
uint8_t sram_cache_read(uint64_t paddr_value);
void sram_cache_write(uint64_t paddr_value, uint8_t data);
#endif

#ifdef USE_PAGETABLE_VA2PA
void pagemap_update_time(uint64_t paddr);
void pagemap_dirty(uint64_t ppn);
#endif


uint64_t virtual_read_data(uint64_t vaddr) {
  uint64_t paddr = va2pa(vaddr, 0);
  uint64_t data = cpu_read64bits_dram(paddr);
  return data;
}

void virtual_write_data(uint64_t vaddr, uint64_t data) {
  uint64_t paddr = va2pa(vaddr, 1);
  cpu_write64bits_dram(paddr, data);
}

void virtual_read_inst(uint64_t vaddr, char *buf) {
  uint64_t paddr = va2pa(vaddr, 0);
  cpu_readinst_dram(paddr, buf);
}

void virtual_write_inst(uint64_t vaddr, const char *str) {
  uint64_t paddr = va2pa(vaddr, 1);
  cpu_writeinst_dram(paddr, str);
}

uint8_t cpu_read8bits_dram(uint64_t paddr) {
  uint64_t val = 0x0;
#ifdef USE_SRAM_CACHE
    val = sram_cache_read(paddr);
#else
  // little-endian
  val = pm[paddr + 0];
#endif

#ifdef USE_PAGETABLE_VA2PA
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
  return val;

}

void cpu_write8bits_dram(uint64_t paddr, uint8_t data) {
#ifdef USE_SRAM_CACHE
    sram_cache_write(paddr, data);
#else
  pm[paddr] = data;
#endif

#ifdef USE_PAGETABLE_VA2PA
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
  pagemap_dirty(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
}

uint64_t cpu_read64bits_dram(uint64_t paddr) {
  uint64_t val = 0x0;
#ifdef USE_SRAM_CACHE
  for (int i = 0; i < 8; ++i) {
    val += (sram_cache_read(paddr + i) << (i * 8));
  }
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
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
  return val;
}

//void print_cache();

void cpu_write64bits_dram(uint64_t paddr, uint64_t data) {
#ifdef USE_SRAM_CACHE
  for (int i = 0; i < 8; ++i) {
    sram_cache_write(paddr + i, (data >> (i * 8)) & 0xff);
  }
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
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
  pagemap_dirty(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
}

void cpu_readinst_dram(uint64_t paddr, char *buf) {
  for (int i = 0;i < MAX_INSTRUCTION_CHAR;++i) {
    buf[i] = (char)pm[paddr+i];
  }
#ifdef USE_PAGETABLE_VA2PA
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
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
#ifdef USE_PAGETABLE_VA2PA
  pagemap_update_time(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
  pagemap_dirty(paddr >> PHYSICAL_PAGE_OFFSET_LENGTH);
#endif
}

void bus_read_cacheline(uint64_t paddr, uint8_t *block) {
  uint64_t dram_base = ((paddr >> SRAM_CACHE_OFFSET_LENGTH)
                        << SRAM_CACHE_OFFSET_LENGTH); // 8 align
  for (int i = 0; i < (1 << SRAM_CACHE_OFFSET_LENGTH); ++i) {
    block[i] = pm[dram_base + i];
  }
}

void bus_write_cacheline(uint64_t paddr, uint8_t *block) {
  uint64_t dram_base = ((paddr >> SRAM_CACHE_OFFSET_LENGTH)
                        << SRAM_CACHE_OFFSET_LENGTH); // 8 align
  for (int i = 0; i < (1 << SRAM_CACHE_OFFSET_LENGTH); ++i) {
    pm[dram_base + i] = block[i];
  }
}
