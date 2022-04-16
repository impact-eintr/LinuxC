#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "../headers/algorithm.h"

#define DEBUG_HASHTABLE

#ifdef DEBUG_HASHTABLE
void print_hashtable(hashtable_t *tab) {
  printf("----------\n");
  printf("global %d\n", tab->globaldepth);
  for (int i = 0; i < tab->num; ++i) {
    hashtable_bucket_t *b = tab->directory[i];
    printf("[%d] local %d = %p: ", i, b->localdepth, b);
    for (int j = 0; j < b->counter; ++j) {
      printf("%s:%ld ,", b->karray[j], b->varray[j]);
    }
    printf("\n");
  }
}
#endif

// BKDR Hash Function
static uint64_t hash_function(char *str) {
  uint32_t seed = 13131; // 31 131 1313 13131 131313 etc..
  uint32_t hash = 0;

  while (*str) {
    hash = hash * seed + (*str++);
  }

  return (hash & 0x7FFFFFFF);
}

static uint64_t lowbits_n(uint64_t num, int length) {
  uint64_t mask = ~(0xffffffffffffffff << length);
  return num & mask;
}

static void split_bucket_full(hashtable_t *tab, hashtable_bucket_t *b);
static void insert_bucket_tail(hashtable_t *tab, hashtable_bucket_t *b, char *key, uint64_t val);

hashtable_t *_hashtable_construct(int size) {
  hashtable_t *tab = malloc(sizeof(hashtable_t));

  tab->globaldepth = 1;
  tab->num = 1 << tab->globaldepth;

  tab->size = size;
  tab->directory = malloc(tab->num * sizeof(hashtable_bucket_t*));
  for (int i = 0;i < tab->num;++i) {
    hashtable_bucket_t *b = malloc(sizeof(hashtable_bucket_t));
    b->localdepth = 1;
    b->counter = 0;
    b->karray = malloc(tab->size * sizeof(char*));
    b->varray = malloc(tab->size * sizeof(uint64_t));

    for (int j = 0;j < tab->size;++j) {
      b->karray[j] = NULL;
      b->varray[j] = 0;
    }

    tab->directory[i] = b;
  }
  return tab;
}

void hashtable_free(hashtable_t *tab) {
  if (tab == NULL) {
    return;
  }

#ifdef DEBUG_DATASTRUCTURE
  printf("free hashtable:\n");
  print_hashtable(tab);
#endif

  for (int i = 0; i < tab->num; ++i) {
    hashtable_bucket_t *b = tab->directory[i];
    if (b == NULL) {
      continue;
    }

    for (int j = 0; j < b->counter; ++j) {
      if (b->karray != NULL && b->karray[j] != NULL) {
        free(b->karray[j]);
        b->karray[j] = NULL;
      }
    }
    if (b->karray != NULL) {
      free(b->karray);
      b->karray = NULL;
    }
    if (b->varray != NULL) {
      free(b->varray);
      b->varray = NULL;
    }
    free(b);
  }
  if (tab->directory != NULL) {
    free(tab->directory);
    tab->directory = NULL;
  }
  free(tab);
}

int hashtable_get(hashtable_t *tab, char *key, uint64_t *valptr) {
  uint64_t hid64 = hash_function(key);

  uint64_t hid = lowbits_n(hid64, tab->globaldepth);
  hashtable_bucket_t *b0 = tab->directory[hid];

  for (int i = 0;i < b0->counter; ++i) {
    if (strcmp(b0->karray[i], key) == 0) {
      // found
      *valptr = b0->varray[i];
      return 1;
    }
  }
  return 0;
}

// 这是一个渐进式扩容
hashtable_t *hashtable_insert(hashtable_t *tab, char *key, uint64_t value) {
  assert(tab != NULL);

  uint64_t hid64 = hash_function(key);
  uint64_t hid = lowbits_n(hid64, tab->globaldepth);

  hashtable_bucket_t *b = tab->directory[hid];
  if (b->counter < tab->size) {
    insert_bucket_tail(tab, b, key, value);
    return tab;
  } else { // 扩容
    if (b->localdepth == tab->globaldepth) {
      // extend the array - double the size
      hashtable_bucket_t **old_array = tab->directory;
      int old_num = tab->num;

      // double
      tab->globaldepth += 1;
      tab->num = 1 << tab->globaldepth;
      tab->directory = malloc(tab->num * sizeof(hashtable_bucket_t*));

      // copy the old array to the new
      for (int i = 0; i < old_num; ++i) {
        if (b != old_array[i]) {
          // this bucket is not changed - point to the same heap address of bucket
          tab->directory[i] = old_array[i];
          // 在对称位置复制一条完全相同的
          tab->directory[i + (1 << (tab->globaldepth - 1))] = old_array[i];
        }
      }

      // for the bucket to be inserted, split
      split_bucket_full(tab, b);

      // finally, insert the input pair
      hid = lowbits_n(hid64, tab->globaldepth);
      insert_bucket_tail(tab, tab->directory[hid], key, value);

      free(old_array);
      return tab;
    } else {
      split_bucket_full(tab, b);
      insert_bucket_tail(tab, tab->directory[hid], key, value);
      return tab;
    }
  }

  return NULL;
}


// 对某个桶及其对称位置的桶重新分配数据
static void split_bucket_full(hashtable_t *tab, hashtable_bucket_t *b) {
  assert(b != NULL);
  assert(b->counter <= tab->size);
  assert(b->localdepth < tab->globaldepth);

  int before_localdepth = b->localdepth;
  int before_counter = b->counter;

  // split into new buckets: b -> b0 and b1

  // b0 use the old heap space
  hashtable_bucket_t *b0 = b;
  b0->counter = 0;
  b0->localdepth = before_localdepth + 1;

  // b1 malloc the new heap space
  hashtable_bucket_t *b1 = malloc(sizeof(hashtable_bucket_t));
  b1->counter = 0;
  b1->localdepth = before_localdepth + 1;
  b1->karray = malloc(tab->size * sizeof(char *));
  b1->varray = malloc(tab->size * sizeof(uint64_t));

  for (int i = 0; i < tab->size; ++i) {
    b1->karray[i] = NULL;
    b1->varray[i] = 0;
  }

  // copy the k-v pairs to the new
  uint64_t hid64 = 0;
  for (int i = 0; i < before_counter; ++i) {
    char *key = b->karray[i];
    uint64_t val = b->varray[i];
    hid64 = hash_function(key);
    uint64_t hid = lowbits_n(hid64, before_localdepth+1);

    if (((hid64 >> before_localdepth) & 0x1) == 1) {
      // move to b1
      b1->karray[b1->counter] = key;
      b1->varray[b1->counter] = val;
      b1->counter += 1;
    } else {
      // b0.counter <= i always true
      // so it's safe to move to b1
      b0->karray[b0->counter] = key;
      b0->varray[b0->counter] = val;
      b0->counter += 1;
    }
  }

  // clear the remaining in b0
  for (int i = b0->counter; i <= tab->size - 1; ++i) {
    b0->karray[i] = NULL;
    b0->varray[i] = 0;
  }

  // till now, all pairs from b have been moved to b0(b) and b1

  // hid64 now is the last hid64, but the low bits should be the same
  uint64_t hid_lowbits = lowbits_n(hid64, before_localdepth);
  for (int highbits = 0; highbits < (1 << (tab->globaldepth - before_localdepth)); ++highbits) {
    // highbits is the permutation of high address bits in hid
    uint64_t hid = hid_lowbits + (highbits << before_localdepth);
    if ((highbits & 0x1) == 1) {
      tab->directory[hid] = b1;
    } else {
      tab->directory[hid] = b0;
    }
  }
}

static void insert_bucket_tail(hashtable_t *tab, hashtable_bucket_t *b,
                               char *key, uint64_t val) {
  assert(b->localdepth <= tab->globaldepth);
  assert(b->counter < tab->size);
  assert(b->karray[b->counter] == NULL);
  assert(b->varray[b->counter] == 0x0);

  // insert the key
  b->karray[b->counter] = malloc((strlen(key) + 1) * sizeof(char));
  strcpy(b->karray[b->counter], key);
  // insert the value
  b->varray[b->counter] = val;
  // update counter
  b->counter++;
}
