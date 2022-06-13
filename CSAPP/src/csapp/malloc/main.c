#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../headers/allocator.h"

#define MALLOC_SIZE 512

int main() {
  heap_init();
  uint64_t arr1 = mem_alloc(sizeof(int) * MALLOC_SIZE);
  int *arr = (int *)&heap[arr1];
  for (int i = 0;i < MALLOC_SIZE;++i) {
    *(arr+i) = i;
  }
  for (int i = 0;i < MALLOC_SIZE;++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  uint64_t arr2 = mem_alloc(sizeof(int) * MALLOC_SIZE);
  arr = (int *)&heap[arr2];
  for (int i = 0;i < MALLOC_SIZE;++i) {
    *(arr+i) = i*2;
  }
  for (int i = 0;i < MALLOC_SIZE;++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  mem_free(arr1);
  mem_free(arr2);
}
