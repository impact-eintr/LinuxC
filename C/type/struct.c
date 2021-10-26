#include <stdio.h>
#include <stdlib.h>

struct A {
  int i;
  char c;
  float f;
};

struct B {
  int i;
  char c;
  float f;
}__attribute__((packed));

int main() {
  struct A a = {123, 'A', 2.22}; // 初始化
  struct A a_ = { .c = 'A', .f = 2.22}; // 部分初始化
  printf("%d %c %.2f\n",a.i, a.c, a.f); // 成员引用
  printf("%d %c %.2f\n",a_.i, a_.c, a_.f); // 成员引用

  struct B b;
  printf("A = %ld\n", sizeof(a));
  printf("B = %ld\n", sizeof(b));
}
