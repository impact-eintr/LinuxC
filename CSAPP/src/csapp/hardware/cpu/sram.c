#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/address.h"
#include "../../headers/common.h"
#include "../../headers/cpu.h"
#include "../../headers/instruction.h"
#include "../../headers/memory.h"

#define CACHE_SIMULATION_VERIFICATION

int cache_hit_count = 0;
int cache_miss_count = 0;
int cache_evict_count = 0;
int dirty_bytes_in_cache_count = 0;
int dirty_bytes_evicted_count = 0;

// How to get byte from cache?
// Sets[CI].Line[CT].Block[CO]

// Sets: array
// Line: hash table
// Block: array

// S[CI].Line[CT].Block[CO]
//
// structure of Line: [State] [Tag] [Block(size = 2<<6)]
//
// state:
// 1. valid / invalid
// 2. clean / dirty

char trace_buf[20];
#define NUM_CACHE_LINE_PER_SET (8)

typedef enum {
  CACHE_LINE_INVALID,
  CACHE_LINE_CLEAN,
  CACHE_LINE_DIRTY
} sram_cacheline_state_t;

typedef struct {
  sram_cacheline_state_t state;
  int time;
  uint64_t tag;
  uint8_t block[(1 << SRAM_CACHE_OFFSET_LENGTH)];
} sram_cacheline_t;

typedef struct {
  sram_cacheline_t lines[NUM_CACHE_LINE_PER_SET];
} sram_cacheset_t;

typedef struct {
  sram_cacheset_t sets[(1 << SRAM_CACHE_OFFSET_LENGTH)];
} sram_cache_t;

static sram_cache_t cache; // initialized with 0 : INVALID

uint8_t sram_cache_read(uint64_t paddr_value) {
  address_t paddr = {
      .paddr_value = paddr_value,
  };

  sram_cacheset_t *set = &cache.sets[paddr.ci];

  // update LRU time
  sram_cacheline_t *victim = NULL;
  sram_cacheline_t *invalid = NULL;
  int max_time = -1;

  for (int i = 0;i < NUM_CACHE_LINE_PER_SET;++i) {
    sram_cacheline_t *line = &(set->lines[i]);
    line->time++;
    if (max_time < line->time) {
      victim = line;
      max_time = line->time;
    }
    if (line->state == CACHE_LINE_INVALID) {
      invalid = line;
    }
  }

  // try cache hit
  for (int i = 0; i < NUM_CACHE_LINE_PER_SET; ++i) {
    sram_cacheline_t *line = &(set->lines[i]);
    if (line->state != CACHE_LINE_INVALID && line->tag == paddr.ct) {
#ifdef CACHE_SIMULATION_VERIFICATION
      // cache hit
      sprintf(trace_buf, "hit");
      cache_hit_count++;
      if (line->state == CACHE_LINE_CLEAN) {
        dirty_bytes_in_cache_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
      }
#endif
      // update LRU time
      line->time = 0;

      return line->block[paddr.co];
    }
  }
#ifdef CACHE_SIMULATION_VERIFICATION
  // cache miss: load from memory
  sprintf(trace_buf, "miss");
  cache_miss_count++;
#endif

  // try to find one free cache line
  if (invalid  != NULL) {
#ifdef CACHE_SIMULATION_VERIFICATION
    bus_read_cacheline(paddr.paddr_value, (uint8_t *)&(invalid->block));
#endif
    // update cache line state
    invalid->state = CACHE_LINE_CLEAN;
    // update LRU
    invalid->time = 0;
    // update tag
    invalid->tag = paddr.ct;

    return invalid->block[paddr.co];
  }

  assert(victim != NULL);

  if (victim->state == CACHE_LINE_DIRTY) {
#ifdef CACHE_SIMULATION_VERIFICATION
    bus_write_cacheline(paddr.paddr_value, (uint8_t*)&(victim->block));
#else
    dirty_bytes_evicted_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
    dirty_bytes_in_cache_count -= (1 << SRAM_CACHE_OFFSET_LENGTH);
#endif
  }
  // After processing dirty pages, the next step is to mark the relevant content in memory
#ifdef CACHE_SIMULATION_VERIFICATION
  sprintf(trace_buf, "miss eviction");
  cache_evict_count++;
#endif
    // update cache line state
    victim->state = CACHE_LINE_INVALID;
#ifdef CACHE_SIMULATION_VERIFICATION
    bus_read_cacheline(paddr.paddr_value, (uint8_t*)&(victim->block));
#endif
    victim->state = CACHE_LINE_CLEAN;

    // update LRU
    victim->time = 0;

    // update tag
    victim->tag = paddr.ct;

    return victim->block[paddr.co];
}

void sram_cache_write(uint64_t paddr_value, uint8_t data) {
  address_t paddr = {
    .paddr_value = paddr_value,
  };
  sram_cacheset_t *set = &(cache.sets[paddr.ci]);

  // update LRU time
  sram_cacheline_t *victim = NULL;
  sram_cacheline_t *invalid = NULL;
  int max_time = -1;
  for (int i = 0; i < NUM_CACHE_LINE_PER_SET; ++i) {
    sram_cacheline_t *line = &(set->lines[i]);
    line->time++;
    if (max_time < line->time) {
      victim = line;
      max_time = line->time;
    }
    if (line->state == CACHE_LINE_INVALID) {
      invalid = line;
    }
  }

  // try cache hit
  for (int i = 0; i < NUM_CACHE_LINE_PER_SET; ++i) {
    sram_cacheline_t *line = &(set->lines[i]);
    if (line->state != CACHE_LINE_INVALID && line->tag == paddr.ct) {
#ifdef CACHE_SIMULATION_VERIFICATION
      // cache hit
      sprintf(trace_buf, "hit");
      cache_hit_count++;
      if (line->state == CACHE_LINE_CLEAN) {
        dirty_bytes_in_cache_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
      }
#endif
      // update LRU time
      line->time = 0;
      line->block[paddr.co] = data;
      line->state = CACHE_LINE_DIRTY;
      return;
    }
  }
#ifdef CACHE_SIMULATION_VERIFICATION
  sprintf(trace_buf, "miss");
  cache_miss_count++;
#endif

  if (invalid != NULL) {
#ifdef CACHE_SIMULATION_VERIFICATION
    bus_read_cacheline(paddr.paddr_value, (uint8_t*)&(invalid->block));
#else
    dirty_bytes_in_cache_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
#endif
    // update cache line state
    invalid->state = CACHE_LINE_DIRTY;
    // update LRU
    invalid->time = 0;
    // update tag
    invalid->tag = paddr.ct;
    // write data
    invalid->block[paddr.co] = data;
    return;
  }
  assert(victim != NULL);

  if (victim->state == CACHE_LINE_DIRTY) {
#ifdef CACHE_SIMULATION_VERIFICATION
    bus_write_cacheline(paddr.paddr_value, (uint8_t*)&(victim->block));
#else
    dirty_bytes_evicted_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
    dirty_bytes_in_cache_count -= (1 << SRAM_CACHE_OFFSET_LENGTH);
#endif
  }
#ifdef CACHE_SIMULATION_VERIFICATION
  sprintf(trace_buf, "miss eviction");
  cache_evict_count++;
  dirty_bytes_in_cache_count += (1 << SRAM_CACHE_OFFSET_LENGTH);
#endif
  // update state
  victim->state = CACHE_LINE_INVALID;
#ifdef CACHE_SIMULATION_VERIFICATION
  bus_read_cacheline(paddr.paddr_value,(uint8_t*)&(victim->block));
#endif
    invalid->state = CACHE_LINE_DIRTY;
    victim->time = 0;
    victim->tag = paddr.ct;
    victim->block[paddr.co] = data;
}

void print_cache() {
  for (int i = 0; i < (1 << SRAM_CACHE_INDEX_LENGTH); ++i) {
    printf("set %x: [ ", i);

    sram_cacheset_t set = cache.sets[i];

    for (int j = 0; j < NUM_CACHE_LINE_PER_SET; ++j) {
      sram_cacheline_t line = set.lines[j];

      char state;
      switch (line.state) {
      case CACHE_LINE_CLEAN:
        state = 'c';
        break;
      case CACHE_LINE_DIRTY:
        state = 'd';
        break;
      case CACHE_LINE_INVALID:
        state = 'i';
        break;
      default:
        state = 'u';
        break;
      }

      printf("(%lx: %c, %d), ", line.tag, state, line.time);
    }

    printf("\b\b ]\n");
  }
}
