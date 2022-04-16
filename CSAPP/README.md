# CSAPP 深入理解计算机系统

## 0x0 无符号整数 二进制补码
<!--没啥好说的-->
int8
0000 0000 0
1000 0000 -128

uint8
0000 0000 0
1000 0000 128
## 0x1 C语言 位操作 树状数组


``` c++
#include <stdlib.h>
#include <stdio.h>

int main() {
  int a = 1<<31;
  printf("%d\n", a+1);
  printf("0x%x\n", a+1);
  return 0;
}

```

``` c++
#include <stdio.h>
#include <stdlib.h>

unsigned LowBit(unsigned x) {
  unsigned a = x & ((~x) + 1);
  return a;
}

#define MAX (1 << 31) - 1

int main() {
  int a = MAX;
  printf("%d\n", a);
  printf("%d\n", (~a));
  printf("%d\n", (~a) + 1);

  int b =0xa;
  printf("0x%x\n", LowBit(b));
  return 0;
}

```

位操作与线段树

``` c++
include <stdio.h>
#include <stdlib.h>

unsigned LowBit(unsigned x) {
  unsigned a = x & ((~x) + 1);
  return a;
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

```

位操作计算某个int的16进制表示是否全是字母

``` c++
>
#include <stdlib.h>

unsigned Letter(unsigned x) {
  unsigned x1 = x & 0x22222222;
  unsigned x2 = x & 0x44444444;
  unsigned x3 = x & 0x88888888;
  return (x3 >> 3) & ((x2 >> 2) | (x1 >> 1));
}

int main() {
  printf("0x%x is Letter 0x%x\n", 0xabcdef, Letter(0xabcdef));
  return 0;
}

```

## 0x2 浮点数表示 类型转换

Floating Point

``` c++
>
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
  if (u <= 0x00ffffff) {
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

```

## 0x3 cpu 寄存器 汇编 指针

`union` 共用体 共用了低地址
``` c++
#ifndef CPU_H_
#define CPU_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// CPU的状态机
typedef struct CPU_STRUCT {
  union {
    struct{
      uint8_t al;
      uint8_t ah;
    };
    uint16_t ax;
    uint32_t eax;
    uint64_t rax;
  };

  union {
    struct{
      uint8_t bl;
      uint8_t bh;
    };
    uint16_t bx;
    uint32_t ebx;
    uint64_t rbx;
  };

  union {
    struct{
      uint8_t cl;
      uint8_t ch;
    };
    uint16_t cx;
    uint32_t ecx;
    uint64_t rcx;
  };

  union {
    struct{
      uint8_t dl;
      uint8_t dh;
    };
    uint16_t dx;
    uint32_t edx;
    uint64_t rdx;
  };

  union {
    struct{
      uint8_t sil;
      uint8_t sih;
    };
    uint16_t si;
    uint32_t esi;
    uint64_t rsi;
  };

  union {
    struct{
      uint8_t dil;
      uint8_t dih;
    };
    uint16_t di;
    uint32_t edi;
    uint64_t rdi;
  };

  union {
    struct{
      uint8_t bpl;
      uint8_t bph;
    };
    uint16_t bp;
    uint32_t ebp;
    uint64_t rbp;
  };

  union {
    struct{
      uint8_t spl;
      uint8_t sph;
    };
    uint16_t sp;
    uint32_t esp;
    uint64_t rsp;
  };

  union {
    struct{
      uint8_t ipl;
      uint8_t iph;
    };
    uint16_t ip;
    uint32_t eip;
    uint64_t rip;
  };
} cpu_t;

#endif // CPU_H_

```


## 0x4 汇编 译码 虚拟地址

## 0x5 汇编模拟器 基本框架

## 0x6

## 0x7

## 0x8

## 0x9

## 0xA

## 0xB

## 0xC

## 0xD

## 0xE

## 0xF

## 0x10

## 0x11

## 0x12

## 0x13

## 0x14

## 0x15

## 0x16

## 0x17

## 0x18

## 0x19

## 0x1A

## 0x1B

## 0x1C

## 0x1D

## 0x1E

## 0x1F

## 0x20

## 0x20

## 0x21

## 0x22

## 0x23

## 0x24

## 0x25

## 0x26

## 0x27

## 0x28

## 0x29

## 0x2A

## 0x2B

## 0x2C

## 0x2D

## 0x2E

## 0x2F

## 0x30

## 0x31

## 0x32

## 0x33

## 0x34

## 0x35

## 0x36

## 0x37

## 0x38

## 0x39

## 0x3A

## 0x3B

## 0x3C

## 0x3D

## 0x3E

## 0x3F

