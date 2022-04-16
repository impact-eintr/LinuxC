#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint64_t add(uint64_t a,uint64_t b) {
  uint64_t c = a + b;
  return  c;
}

int main() {
  uint64_t a = 0x1234;
  uint64_t b = 0xabcd0000;
  uint64_t c = add(a, b);
  return 0;
}
