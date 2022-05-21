#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  MODIFIED,  // exclusive dirty,global 1
  EXCLUSIVE, // exclusive clean,global 1
  SHARED,    // shared cleanm global >= 2
  INVALID,   // invalid
} state_t;

typedef struct {
  state_t state;
  int value;
} line_t;

#define NUM_PROCESSOR (16)

// c0 c1 c2 ..... cn
// [] [] [] [...] []
// We assume that each cpu cache can only store one value
line_t cache[NUM_PROCESSOR];

int mem_value = 15213; // global value

int check_state() {
  int m_count = 0;
  int e_count = 0;
  int s_count = 0;
  int i_count = 0;
  for (int i = 0; i < NUM_PROCESSOR; ++i) {
    if (cache[i].state == MODIFIED) {
      m_count++;
    } else if (cache[i].state == EXCLUSIVE) {
      e_count++;
    } else if (cache[i].state == SHARED) {
      s_count++;
    } else if (cache[i].state == INVALID) {
      i_count++;
    }
  }
#ifdef DEBUG
  printf("M %d\t E %d\t S %d\t I %d\n", m_count, e_count, s_count, i_count);
#endif

  if ((m_count == 1 && i_count == (NUM_PROCESSOR - 1)) ||
      (e_count == 1 && i_count == (NUM_PROCESSOR - 1)) ||
      (s_count >= 2 && i_count == (NUM_PROCESSOR - s_count)) ||
      (i_count == NUM_PROCESSOR)) {
    return 1;
  }
  return 0;
}

// i: index
int read_cacheline(int i, int *read_value) {
  if (cache[i].state == MODIFIED) {
#ifdef DEBUG
    printf("[%d] read hit; dirty value %d\n", i, cache[i].value);
#endif
    *read_value = cache[i].value;
    return 1;
  } else if (cache[i].state == EXCLUSIVE) {
#ifdef DEBUG
    printf("[%d] read hit; exclustive clean value %d\n", i, cache[i].value);
#endif
    *read_value = cache[i].value;
    return 1;
  } else if (cache[i].state == SHARED) {
#ifdef DEBUG
    printf("[%d] read hit; shared clean value %d\n", i, cache[i].value);
#endif
    *read_value = cache[i].value;
    return 1;
  } else {
    // read miss
    // bus boardcast read miss
    for (int j = 0; j < NUM_PROCESSOR; j++) {
      if (i != j) {
        if (cache[j].state == MODIFIED) {
          // write back me_value modified
          // there are exactly 2 copies in processors
          mem_value = cache[j].value;
          cache[j].state = SHARED;
          // update read miss cache
          cache[i].state = SHARED;
          cache[i].value = cache[j].value;

          *read_value = cache[i].value;
#ifdef DEBUG
          printf("[%d] read miss; [%d] supplies dirty value %d; write back; "
                 "s_count == 2\n",
                 i, j, cache[i].value);
#endif
          return 1;
        } else if (cache[j].state == EXCLUSIVE) {
          // no memory transaction
          cache[i].state = SHARED;
          cache[i].value = cache[j].value;
          // there are exactly 2 copies in processors
          cache[j].state = SHARED;
          *read_value = cache[i].value;
#ifdef DEBUG
          printf("[%d] read miss; [%d] supplies clean value %d; s_count >= 3\n",
                 i, j, cache[i].value);
#endif
          return 1;
        } else if (cache[j].state == SHARED) {
          // >= 3
          cache[i].state = SHARED;
          cache[i].value = cache[j].value;
          *read_value = cache[i].value;
#ifdef DEBUG
          printf("[%d] read miss; [%d] supplies clean value %d; s_count >= 3\n",
                 i, j, cache[i].value);
#endif
          return 1;
        }
      }
    }
    // all other are invalid
    cache[i].state = EXCLUSIVE;
    cache[i].value = mem_value;
    *read_value = cache[i].value;
#ifdef DEBUG
    printf("[%d] read miss; mem supplies clean value %d; e_count == 1\n", i,
           cache[i].value);
#endif
    return 1;
  }
  return 0;
}
#define DEBUG
int write_cacheline(int i, int write_value) {
  if (cache[i].state == MODIFIED) {
    // write hit
    cache[i].value = write_value;
#ifdef DEBUG
    printf("[%d] write hit; update to value %d\n", i, cache[i].value);
#endif
    return 1;
  } else if (cache[i].state == EXCLUSIVE) {
    cache[i].state = MODIFIED;
    cache[i].value = write_value;
#ifdef DEBUG
    printf("[%d] write hit; update to value %d\n", i, cache[i].value);
#endif
    return 1;
  } else if (cache[i].state == SHARED) {
    // boardcast write invalid
    for (int j = 0; j < NUM_PROCESSOR; ++j) {
      if (j != i) {
        cache[j].state = INVALID;
        cache[j].value = 0;
      }
    }
    cache[i].state = MODIFIED;
    cache[i].value = write_value;
#ifdef DEBUG
    printf("[%d] write hit;boardcast invalid;update to value %d\n", i,
           cache[i].value);
#endif
    return 1;
  } else if (cache[i].state == INVALID) {
    for (int j = 0; j < NUM_PROCESSOR; ++j) {
      if (i != j) {
        if (cache[j].state == MODIFIED) {
          // write back
          mem_value = cache[j].value;
          // invalid old cache line
          cache[j].state = INVALID;
          cache[j].value = 0;
          // write allocate
          cache[i].value = mem_value;
          // update to modified
          cache[i].state = MODIFIED;
          cache[i].value = write_value;
#ifdef DEBUG
          printf(
              "[%d] write miss; boardcast invalid to M; update to value %d\n",
              i, cache[i].value);
#endif
          return 1;
        } else if (cache[j].state == EXCLUSIVE) {
          cache[j].state = INVALID;
          cache[j].value = 0;

          cache[i].state = MODIFIED;
          cache[i].value = write_value;
#ifdef DEBUG
          printf(
              "[%d] write miss; boardcast invalid to E; update to value %d\n",
              i, cache[i].value);
#endif
          return 1;
        } else if (cache[j].state == SHARED) {
          for (int k = 0; k < NUM_PROCESSOR; ++k) {
            if (i != k) {
              cache[k].state = INVALID;
              cache[k].value = 0;
            }
          }
          cache[i].state = MODIFIED;
          cache[i].value = write_value;
#ifdef DEBUG
          printf(
              "[%d] write miss; boardcast invalid to S; update to value %d\n",
              i, cache[i].value);
#endif
          return 1;
        }
      }
    }
    // all other are invalid
    // write allocate
    cache[i].value = mem_value;
    cache[i].state = MODIFIED;
    cache[i].value = write_value;
#ifdef DEBUG
    printf("[%d] write miss;all invalid;update to value %d\n", i,
           cache[i].value);
#endif
    return 1;
  }
  return 0;
}

// LRU
// int return - if this event is related with target physical address
int evict_cacheline(int i) {
  if (cache[i].state == MODIFIED) {
    // write back
    mem_value = cache[i].value;
    cache[i].state = INVALID;
    cache[i].value = 0;
    return 1;
  } else if (cache[i].state == EXCLUSIVE) {
    cache[i].state = INVALID;
    cache[i].value = 0;
    return 1;
  } else if (cache[i].state == SHARED) {
    cache[i].state = INVALID;
    cache[i].value = 0;

    int s_count = 0;
    int last_s = -1;
    for (int j = 0;j < NUM_PROCESSOR;++j) {
      if (cache[j].state == SHARED) {
        last_s = j;
        s_count++;
      }
    }
    if (s_count == 1) {
      cache[last_s].state = EXCLUSIVE;
    }
    return 1;
  }
  return 0;
}

void print_cacheline() {

  for (int i = 0; i < NUM_PROCESSOR; ++i) {
    char c;

    switch (cache[i].state) {
    case MODIFIED:
      c = 'M';
      break;
    case EXCLUSIVE:
      c = 'E';
      break;
    case SHARED:
      c = 'S';
      break;
    case INVALID:
      c = 'I';
      break;
    default:
      c = '?';
    }
    printf("\t[%d]\tstate %c\tvalue %d\n", i, c, cache[i].value);
  }
  printf("\t\t\t\tmem value %d\n", mem_value);
}

int main() {
  srand(123456);
  int read_value;
  for (int i = 0;i < NUM_PROCESSOR;++i) {
    cache[i].state = INVALID;
    cache[i].value = 0;
  }
  print_cacheline();

  for (int i = 0;i < 100000;++i) {
    int core_index = rand() % NUM_PROCESSOR;
    int op = rand() % 3;
    int do_print = 0;
    if (op == 0) {
      do_print = read_cacheline(core_index, &read_value);
      printf("read [%d]\n", core_index);
    } else if (op == 1) {
      do_print = write_cacheline(core_index, rand() % 1000);
      printf("write [%d]\n", core_index);
    } else if (op == 2) {
      do_print = evict_cacheline(core_index);
      printf("evict [%d]\n", core_index);
    }

    if (do_print) {
      print_cacheline();
    }
    if (check_state() == 0) {
      exit(1);
    }
  }

  return 0;
}
