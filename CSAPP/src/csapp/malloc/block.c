#include <assert.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

uint64_t heap_start_vaddr;
uint64_t heap_end_vaddr;
uint8_t heap[HEAP_MAX_SIZE];

uint64_t round_up(uint64_t x, uint64_t n) { return n * ((x + n - 1) / n); }

// Block Operations
#define AF_BIT (0)
#define P8_BIT (1)
#define B8_BIT (2)

static void set_bit(uint64_t vaddr, int bit_offset) {
  uint32_t vector = 1 << bit_offset;

  assert((vaddr & 0x3) == 0); // 8 bits align
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  *(uint32_t *)&heap[vaddr] |= (~vector); // set this bit 1
}

static void reset_bit(uint64_t vaddr, int bit_offset) {
  uint32_t vector = 1 << bit_offset;

  assert((vaddr & 0x3) == 0);
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  *(uint32_t *)&heap[vaddr] &= (~vector); // set this bit 0
}

static int is_bit_set(uint64_t vaddr, int bit_offset) {
  assert((vaddr & 0x3) == 0);
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  return (*(uint64_t *)&heap[vaddr] >> bit_offset) & 1;
}

// [header|(no payload)|footer]
static void check_block8_correctness(uint64_t vaddr) {
  if (vaddr == NIL) {
    return;
  }
  assert(vaddr % 4 == 0);
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  if (vaddr % 8 == 4) {
    // header
    assert(is_bit_set(vaddr, B8_BIT) == 1);

    uint64_t next_header = vaddr + 8;
    // B8 cannot be epilogue
    assert(next_header <= get_epilogue());
    assert(is_bit_set(next_header, P8_BIT) == 1);

    if (get_allocated(vaddr) == ALLOCATED) {
      // NOTE (*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) is blocksize of this block
      assert((*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) == 8);
    }
  } else if (vaddr % 8 == 0) {
    // footer
    uint64_t next_header = vaddr + 4;
    assert(next_header <= get_epilogue());
    assert(is_bit_set(next_header, P8_BIT) == 1);
    vaddr -= 4;
    assert(is_bit_set(vaddr, P8_BIT) == 1);

    if (get_allocated(vaddr) == ALLOCATED) {
      // NOTE (*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) is blocksize of this block
      assert((*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) == 8);
    }
  } else {
    assert(0);
  }
}

#define DEBUG_MALLOC

// check  [header|(no payload)|footer]
static int is_block8(uint64_t vaddr) {
  if (vaddr == NIL || vaddr == get_epilogue()) {
    return 0;
  }
  assert(get_prologue() <= vaddr && vaddr < get_epilogue());

  if (vaddr % 8 == 4) {
    // header
    if (is_bit_set(vaddr, B8_BIT) == 1) {
#ifdef DEBUG_MALLOC
      check_block8_correctness(vaddr);
#endif
      return 1;
    }
  } else if (vaddr % 8 == 0) {
    // footer
    uint64_t next_header = vaddr + 4;
    if (is_bit_set(next_header, P8_BIT) == 1) {
#ifdef DEBUG_MALLOC
      check_block8_correctness(vaddr - 4);
#endif
      return 1;
    }
  }
  return 0;
}
// operations for all blocks
// applicapable for both header & footer
uint32_t get_blocksize(uint64_t header_vaddr) {
  if (header_vaddr == NIL) {
    return 0;
  }

  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert((header_vaddr & 0x3) == 0x0); // header & footer should be 4 bytes alignment

  if (is_block8(header_vaddr) == 1) {
#ifdef DEBUG_MALLOC
      check_block8_correctness(header_vaddr);
#endif
      return 8;
  } else {
    // B8 is unset - an ordinary block
    return (*(uint32_t *)&heap[header_vaddr] & 0xFFFFFFF8);
  }
}

void set_blocksize(uint64_t header_vaddr, uint32_t blocksize) {
  if (header_vaddr == NIL) {
    return;
  }
}

uint32_t get_allocated(uint64_t header_vaddr);
void set_allocated(uint64_t header_vaddr, uint32_t allocated);

uint32_t get_payload(uint64_t vaddr);
uint64_t get_header(uint64_t vaddr);
uint64_t get_footer(uint64_t vaddr);
