#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t uint2float(uint32_t u) {

  if (0x00000000 == u) {
    return  0x00000000;
  }

  int n = 31;
  while (0 <= n && (((u >> n) & 0x1) == 0x0)) {
    n--;
  }

  uint32_t e, f;
  // <= 0000 0000 1.111 1111 1111 1111 1111 1111 小数点后没有超过23 可以表示
  if (u < 0x00ffffff) {
    uint32_t mask = 0xffffffff >> (32 - n);
    f = (u & mask) << (23 - n);
    e = n + 127;
    return (e << 23) | f;
  } else { // >= 0000 0001. 0000 0000 0000 0000 0000 0000 小数点后超过23 需要近似
    // need rounding
    uint64_t a = 0;
    a += u;

    // compute g, r, s;
    uint32_t g = (a >> (n - 23) & 0x1);
    uint32_t r = (a >> (n - 24) & 0x1);
    uint32_t s = 0x0;
    for (int j = 0;j < n - 24;++j) {
      s = s | ((u >> j) & 0x1);
    }
    // compute carry
    a = a >> (n-23);
    // 0    1    ?    ... ?
    // [24] [23] [22] ... [0]
    if ((r&(g|s)) == 0x1) {
      a = a + 1;
    }

    if ((a >> 23) == 0x1) {
      //  0    1    ?   ...  ?
      // [24] [23] [22] ... [0]
      f = a & 0x007fffff;
      e = n + 127;
      return (e << 23) | f;
    } else if ((a >> 23) == 0x2) { // 最大值
      //  1    0    0   ...  0
      // [24] [23] [22] ... [0]
      e = n + 1 + 127;
      return (e << 23);
    }
  }
  // inf as default error
  return 0x7f800000;
}

// 用一个 uint32 模拟 float32
int main() {
  printf("%x\n", uint2float(0x0000000f));
}
