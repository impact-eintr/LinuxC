#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../headers/allocator.h"

#define MALLOC_SIZE 1024 * 4

int main() {
  heap_init();
  uint64_t arr_p = mem_alloc(sizeof(int) * MALLOC_SIZE);
  int *arr = (int *)&heap[arr_p];

  for (int i = 0;i < MALLOC_SIZE;++i) {
    *(arr+i) = i;
  }
  for (int i = 0;i < MALLOC_SIZE;++i) {
    printf("%d ", arr[i]);
  }
  mem_free(arr_p);
}
