#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <stdint.h>

extern uint64_t heap_start_vaddr;
extern uint64_t heap_end_vaddr;

#define HEAP_MAX_SIZE (2 << 20)

extern uint8_t heap[HEAP_MAX_SIZE];

#define FREE (0)
#define ALLOCATED (1)
#define NIL (0)

#define MIN_EXPLICT_FREE_LIST_BLOCKSIZE (16)
#define MIN_REDBLACK_TREE_BLOCKSIZE (24)

// to allocate one physical page for page
uint32_t exten_heap(uint32_t size);
void os_syscall_brk();

// round up
uint64_t round_up(uint64_t x, uint64_t n);

// operations for all blocks
uint32_t get_blocksize(uint64_t header_vaddr);
void set_blocksize(uint64_t header_vaddr, uint32_t blocksize);

uint32_t get_allocated(uint64_t header_vaddr);
void set_allocated(uint64_t header_vaddr, uint32_t allocated);

uint32_t get_payload(uint64_t vaddr);
uint64_t get_header(uint64_t vaddr);
uint64_t get_footer(uint64_t vaddr);

// operations for heap linked list
uint64_t get_nextheader(uint64_t vaddr);
uint64_t get_prevheader(uint64_t vaddr);

uint64_t get_prologue();
uint64_t get_epilogue();

uint64_t get_firstblock();
uint64_t get_lastblock();

int is_lastblock(uint64_t vaddr);
int is_firstblock(uint64_t vaddr);

// for free block as data structure
uint64_t get_field32_block_ptr(uint64_t header_vaddr, uint32_t min_blocksize, uint32_t offset);
void set_field32_block_ptr(uint64_t header_vaddr, uint64_t block_ptr, uint32_t min_blocksize, uint32_t offset);

// interface
int heap_init();
uint64_t mem_alloc(uint32_t size);
uint64_t mem_aligned_alloc(uint32_t size, uint64_t aligned);
void mem_free(uint64_t payload_vaddr);

#endif // ALLOCATOR_H_
