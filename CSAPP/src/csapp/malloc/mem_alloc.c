#include <assert.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/allocator.h"

// Operating System Implemented
void os_syscall_brk() {
  // TODO pagefault
}

uint32_t extend_heap(uint32_t size) {
  // round up to page aligment
  size = (uint32_t)round_up((uint64_t)size, 4096);
  if (heap_end_vaddr - heap_start_vaddr + size <= HEAP_MAX_SIZE) {
    os_syscall_brk();
    heap_end_vaddr += size;
  } else {
    return 0;
  }

  uint64_t epilogue = get_epilogue();
  set_allocated(epilogue, ALLOCATED);
  set_blocksize(epilogue, 0);
  return size;
}

// Free Block Mamagement Implemented
#ifdef IMPLICIT_FREE_LIST
int implicit_list_initialize_free_block();
uint64_t implicit_list_search_free_block(uint32_t payload_size, uint32_t *alloc_blocksize);
int implicit_list_insert_free_block(uint64_t free_header);
int implicit_list_delete_free_block(uint64_t free_header);
void implicit_list_check_free_block();
#endif

#ifdef EXPLICIT_FREE_LIST
int explicit_list_initialize_free_block();
uint64_t explicit_list_search_free_block(uint32_t payload_size, uint32_t *alloc_blocksize);
int explicit_list_insert_free_block(uint64_t free_header);
int explicit_list_delete_free_block(uint64_t free_header);
void explicit_list_check_free_block();
#endif

#ifdef REDBLACK_TREE
int redblack_tree_initialize_free_block();
uint64_t redblack_tree_search_free_block(uint32_t payload_size, uint32_t *alloc_blocksize);
int redblack_tree_insert_free_block(uint64_t free_header);
int redblack_tree_delete_free_block(uint64_t free_header);
void redblack_tree_check_free_block();
#endif

static int initialize_free_block() {
#ifdef IMPLICIT_FREE_LIST
  return implicit_list_initialize_free_block();
#elif defined (EXPLICIT_FREE_LIST)
  return explicit_list_initialize_free_block();
#elif defined (REDBLACK_TREE)
  return redblack_tree_initialize_free_block();
#endif
  assert(0);
}

static uint64_t search_free_block(uint32_t payload_size, uint32_t *alloc_blocksize) {
#ifdef IMPLICIT_FREE_LIST
  return implicit_list_search_free_block(payload_size, alloc_blocksize);
#elif defined (EXPLICIT_FREE_LIST)
  return explicit_list_search_free_block(payload_size, alloc_blocksize);
#elif defined (REDBLACK_TREE)
  return redblack_tree_search_free_block(payload_size, alloc_blocksize);
#endif
  assert(0);
}

static int insert_free_block(uint64_t free_header) {
#ifdef IMPLICIT_FREE_LIST
  return implicit_list_insert_free_block(free_header);
#elif defined (EXPLICIT_FREE_LIST)
  return explicit_list_insert_free_block(free_header);
#elif defined (REDBLACK_TREE)
  return redblack_tree_insert_free_block(free_header);
#endif
  assert(0);
}

static int delete_free_block(uint64_t free_header) {
#ifdef IMPLICIT_FREE_LIST
  return implicit_list_delete_free_block(free_header);
#elif defined (EXPLICIT_FREE_LIST)
  return explicit_list_delete_free_block(free_header);
#elif defined (REDBLACK_TREE)
  return redblack_tree_delete_free_block(free_header);
#endif
  assert(0);
}

static void check_free_block() {

}

// ------------------------------
//        Malloc And Free
// ------------------------------

// Util
void check_heap_correctness();

static uint64_t merge_blocks_as_free(uint64_t low, uint64_t high) {

}

static uint64_t try_alloc_with_splitting(uint64_t block_vaddr, uint32_t request_blocksize) {

}

static uint64_t try_extend_heap_to_alloc(uint32_t size) {

}

// interface
int heap_init() {
  // reset all to 0
  for (int i = 0; i < HEAP_MAX_SIZE / 8; i += 8) {
    *(uint64_t *)&heap[i] = 0;
  }

  heap_start_vaddr = 0;
  heap_end_vaddr = 4096;

  // set prologue block
  uint64_t prologue_header = get_prologue();
  set_allocated(prologue_header, ALLOCATED);
  set_blocksize(prologue_header, 8);

  uint64_t prologue_footer = prologue_header + 4;
  set_allocated(prologue_footer, ALLOCATED);
  set_blocksize(prologue_footer, 8);

  // set epilogue block
  // it is a footer only
  uint64_t epilogue = get_epilogue();
  set_allocated(epilogue, ALLOCATED);
  set_blocksize(epilogue, 0);

  // set the block size & allocated of the only regular block
  uint64_t first_header = get_firstblock();
  set_allocated(first_header, FREE);
  set_blocksize(first_header, 4096 - 4 - 8 - 4);

  uint64_t first_footer = get_footer(first_header);
  set_allocated(first_footer, FREE);
  set_blocksize(first_footer, 4096 - 4 - 8 - 4);

  initialize_free_block();

  return 1;
}

uint64_t mem_alloc(uint32_t size) {
  assert(0 < size && size < HEAP_MAX_SIZE - 4 - 8 - 4);

  uint32_t alloc_blocksize = 0;
  uint64_t payload_header = search_free_block(size, &alloc_blocksize);
  uint64_t payload_vaddr = NIL;

  if (payload_header != NIL) {
    payload_vaddr = try_alloc_with_splitting(payload_header, alloc_blocksize);
    assert(payload_vaddr != NIL);
  } else {
    payload_vaddr = try_extend_heap_to_alloc(alloc_blocksize);
  }
#ifdef DEBUG_MALLOC
  check_heap_correctness();
  check_free_block();
#endif
  return payload_vaddr;
}

void mem_free(uint64_t payload_vaddr) {}

// ------------------------------------
// Debugging and Correctness Checking
// ------------------------------------
void check_heap_correctness() {}

static void block_info_print(uint64_t h) {}

static void heap_blocks_print() {}
