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

// set this bit 1
static void set_bit(uint64_t vaddr, int bit_offset) {
  uint32_t vector = 1 << bit_offset;

  assert((vaddr & 0x3) == 0); // 8 bits align
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  *(uint32_t *)&heap[vaddr] |= vector;
}

// set this bit 0
static void reset_bit(uint64_t vaddr, int bit_offset) {
  uint32_t vector = 1 << bit_offset;

  assert((vaddr & 0x3) == 0);
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  *(uint32_t *)&heap[vaddr] &= (~vector);
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
      // NOTE (*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) is blocksize of this
      // block
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
      // NOTE (*(uint32_t *)&heap[vaddr] & 0xFFFFFFF8) is blocksize of this
      // block
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
  assert((header_vaddr & 0x3) ==
         0x0); // header & footer should be 4 bytes alignment

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
  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert((header_vaddr & 0x3) == 0x0);
  assert((blocksize & 0x7) == 0x0);

  uint64_t next_header_vaddr;
  if (blocksize == 8) {
    // small block is special
    if (header_vaddr % 8 == 0) {
      // don not set footer of small block
      // reset to header
      header_vaddr = header_vaddr - 4;
    }
    next_header_vaddr = header_vaddr + 8;

    set_bit(header_vaddr, B8_BIT);
    if (next_header_vaddr <= get_epilogue()) {
      // free 8-bytes does not set block size
      set_bit(next_header_vaddr, P8_BIT);
    }
    if (get_allocated(header_vaddr) == FREE) {
      return;
    }
  } else {
    if (header_vaddr % 8 == 4) {
      next_header_vaddr = header_vaddr + blocksize;
    } else {
      next_header_vaddr = header_vaddr + 4;
    }

    reset_bit(header_vaddr, B8_BIT);
    if (next_header_vaddr <= get_epilogue()) {
      reset_bit(next_header_vaddr, P8_BIT);
    }
  }
  *(uint32_t *)&heap[header_vaddr] &= 0x00000007; // reset size
  *(uint32_t *)&heap[header_vaddr] |= blocksize;  // set size

#ifdef DEBUG_MALLOC
  if (blocksize == 8) {
    check_block8_correctness(header_vaddr);
  }
#endif
}

uint32_t get_allocated(uint64_t header_vaddr) {
  if (header_vaddr == NIL) {
    return ALLOCATED;
  }
  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert((header_vaddr & 0x3) == 0x0);

  if (header_vaddr % 8 == 0) {
    // footer
    uint64_t next_header_vaddr = header_vaddr + 4;
    if (next_header_vaddr <= get_epilogue()) {
      if (is_bit_set(next_header_vaddr, P8_BIT) == 1) {
        // current block is 8-byte, no footer check instead
        header_vaddr -= 4;
#ifdef DEBUG_MALLOC
        check_block8_correctness(header_vaddr);
#endif
      }
    } else {
      assert(0);
    }
  }
  // header
  return (*(uint32_t *)&heap[header_vaddr] & 0x1);
}

void set_allocated(uint64_t header_vaddr, uint32_t allocated) {
  if (header_vaddr == NIL) {
    return;
  }
  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert((header_vaddr & 0x3) == 0x0);
  if (header_vaddr % 8 == 0) {
    // footer
    uint64_t next_header_vaddr = header_vaddr + 4;
    if (next_header_vaddr <= get_epilogue()) {
      // check P8 bit of next
      if (is_bit_set(next_header_vaddr, P8_BIT) == 1) {
        header_vaddr -= 4;
#ifdef DEBUG_MALLOC
        check_block8_correctness(header_vaddr);
#endif
      }
    } else {
      assert(0);
    }
  }
  *(uint32_t *)&heap[header_vaddr] &= 0xFFFFFFFE; // 0xFFFFFFF1110
  *(uint32_t *)&heap[header_vaddr] |= (allocated & 0x1);
}

uint32_t get_payload(uint64_t vaddr) {
  if (vaddr == NIL) {
    return NIL;
  }
  assert(get_firstblock() <= vaddr && vaddr < get_epilogue());
  assert((vaddr & 0x3) == 0);
  return round_up(vaddr, 8);
}

uint64_t get_header(uint64_t vaddr) {
  if (vaddr == NIL) {
    return NIL;
  }
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());
  // 1 starting address of the block (8 * n + 4)
  // 2 starting address of the payload (8 * m)
  assert((vaddr & 0x3) == 0);

  return round_up(vaddr, 8) - 4;
}

uint64_t get_footer(uint64_t vaddr) {
  if (vaddr == NIL) {
    return NIL;
  }
  assert(get_prologue() <= vaddr && vaddr < get_epilogue());
  // 1 starting address of the block (8 * n + 4)
  // 2 starting address of the payload (8 * m)
  //assert((vaddr % 8) == 4); // TODO WHY we can not get the footer by payload ? I don not think so
  assert((vaddr & 0x3) == 0);

  uint64_t header_vaddr = get_header(vaddr);
  uint64_t footer_vaddr = header_vaddr + get_blocksize(header_vaddr) - 4;

  assert(get_firstblock() < footer_vaddr && footer_vaddr < get_epilogue());
  assert(footer_vaddr % 8 == 0);
  return footer_vaddr;
}

// operations for heap linked list
uint64_t get_nextheader(uint64_t vaddr) {
  if (vaddr == NIL || vaddr == get_epilogue()) {
    return NIL;
  }
  assert(get_prologue() <= vaddr && vaddr < get_epilogue());

  // vaddr can be:
  // 1 starting address of the block (8 * n + 4)
  // 2 starting address of the payload (8 * m)
  uint64_t header_vaddr = get_header(vaddr);
  uint32_t blocksize = get_blocksize(header_vaddr);

  uint64_t next_header_vaddr = header_vaddr + blocksize;
  assert(get_firstblock() < next_header_vaddr && next_header_vaddr <= get_epilogue());

  return next_header_vaddr;
}

uint64_t get_prevheader(uint64_t vaddr) {
  if (vaddr == NIL || vaddr ==get_prologue()) {
    return NIL;
  }
  assert(get_prologue() <= vaddr && vaddr <= get_epilogue());

  uint64_t header_vaddr = get_header(vaddr);
  uint32_t header_value = *(uint32_t *)&heap[header_vaddr];
  uint64_t prev_header_vaddr;

  // check P8 bit 0010
  if (is_bit_set(header_vaddr, P8_BIT) == 1) {
    // previous block is 8-byte block
    prev_header_vaddr = header_vaddr - 8;
#ifdef DEBUG_MALLOC
    check_block8_correctness(prev_header_vaddr);
#endif
    return prev_header_vaddr;
  } else {
    uint64_t prev_footer_vaddr = header_vaddr - 4;
    uint32_t prev_blocksize = get_blocksize(prev_footer_vaddr);

    prev_header_vaddr = header_vaddr - prev_blocksize;
    assert(get_prologue() <= prev_header_vaddr && prev_header_vaddr < get_epilogue());
    assert(get_blocksize(prev_header_vaddr) == get_blocksize(prev_footer_vaddr));
    assert(get_allocated(prev_header_vaddr) == get_allocated(prev_footer_vaddr));

    return prev_header_vaddr;
  }
}

uint64_t get_prologue() {
  assert(heap_end_vaddr > heap_start_vaddr);
  assert((heap_end_vaddr - heap_start_vaddr) % 4096 == 0);
  assert(heap_start_vaddr % 4096 == 0);

  // 4 for the not in use
  return heap_start_vaddr + 4;
}

uint64_t get_epilogue() {
  assert(heap_end_vaddr > heap_start_vaddr);
  assert((heap_end_vaddr - heap_start_vaddr) % 4096 == 0);
  assert(heap_start_vaddr % 4096 == 0);

  // 4 for the not in use
  return heap_end_vaddr - 4;
}

uint64_t get_firstblock() {
  assert(heap_end_vaddr > heap_start_vaddr);
  assert((heap_end_vaddr - heap_start_vaddr) % 4096 == 0);
  assert(heap_start_vaddr % 4096 == 0);

  // 4 for the not in use
  // 8 for the prologue block
  return get_prologue() + 8;
}

uint64_t get_lastblock() {
  assert(heap_end_vaddr > heap_start_vaddr);
  assert((heap_end_vaddr - heap_start_vaddr) % 4096 == 0);
  assert(heap_start_vaddr % 4096 == 0);

  uint64_t epilogue_header = get_epilogue();
  return get_prevheader(epilogue_header);
}

int is_lastblock(uint64_t vaddr) {
  if (vaddr == NIL) {
    return 0;
  }

  // vaddr can be:
  // 1. starting address of the block (8 * n + 4)
  // 2. starting address of the payload (8 * m)
  assert(get_firstblock() <= vaddr && vaddr < get_epilogue());
  assert((vaddr & 0x3) == 0x0);

  uint64_t header_vaddr = get_header(vaddr);
  uint32_t blocksize = get_blocksize(header_vaddr);

  if (header_vaddr + blocksize == get_epilogue()) {
    // it is the last block
    // it does not have any footer
    return 1;
  }

  // no, it's not the last block
  // it should have footer
  return 0;
}

int is_firstblock(uint64_t vaddr) {
  if (vaddr == NIL) {
    return 0;
  }

  // vaddr can be:
  // 1. starting address of the block (8 * n + 4)
  // 2. starting address of the payload (8 * m)
  assert(get_firstblock() <= vaddr && vaddr < get_epilogue());
  assert((vaddr & 0x3) == 0x0);

  uint64_t header_vaddr = get_header(vaddr);

  if (header_vaddr == get_firstblock()) {
    // it is the last block
    // it does not have any footer
    return 1;
  }

  // no, it's not the last block
  // it should have footer
  return 0;
}

// for free block as data structure
uint64_t get_field32_block_ptr(uint64_t header_vaddr, uint32_t min_blocksize, uint32_t offset) {
  if (header_vaddr == NIL) {
    return NIL;
  }

  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert(header_vaddr % 4 == 0);
  assert(get_blocksize(header_vaddr) >= min_blocksize);
  assert(offset % 4 == 0);

  uint32_t vaddr_32 = *(uint32_t *)&heap[header_vaddr + offset];
  return (uint64_t)vaddr_32;
}

void set_field32_block_ptr(uint64_t header_vaddr, uint64_t block_ptr, uint32_t min_blocksize, uint32_t offset) {
  if (header_vaddr == NIL) {
    return;
  }

  assert(get_prologue() <= header_vaddr && header_vaddr <= get_epilogue());
  assert(header_vaddr % 4 == 0);
  assert(get_blocksize(header_vaddr) >= min_blocksize);

  assert(block_ptr == NIL || (get_firstblock() <= block_ptr && block_ptr <= get_lastblock()));
  assert(block_ptr == NIL || block_ptr % 8 == 4);
  assert(block_ptr == NIL || get_blocksize(block_ptr) >= min_blocksize);
  assert(offset % 4 == 0);

  // actually a 32-bits pointer
  assert((block_ptr >> 32) == 0);
  *(uint32_t *)&heap[header_vaddr + offset] = (uint32_t)(block_ptr & 0xFFFFFFFF);
}
