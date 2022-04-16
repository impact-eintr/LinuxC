#include <stdio.h>
#include <stdlib.h>

unsigned LowBit(unsigned x) {
  unsigned a = x & ((~x) + 1);
  return a;
}

#define MAX (1 << 31) - 1

unsigned Letter(unsigned x) {
  unsigned x1 = x & 0x22222222;
  unsigned x2 = x & 0x44444444;
  unsigned x3 = x & 0x88888888;
  return (x3 >> 3) & ((x2 >> 2) | (x1 >> 1));
}

int main() {
  printf("0x%x\n", LowBit(12)); // 1100

  unsigned n = 7;
  // segment tree
  printf("S[%u] ==", n);
  printf(" T[%u] +", n);
  n = n - LowBit(n);
  printf(" T[%u] +", n);
  n = n - LowBit(n);
  printf(" T[%u] +", n);
  n = n - LowBit(n);
  printf(" T[%u]\n", n);

  return 0;
}
