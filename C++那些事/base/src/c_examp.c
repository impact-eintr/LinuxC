#include <stdio.h>

typedef void (*pf)();

typedef struct _A {
  pf _f;
}A;

typedef struct _B {
  A _b; // 继承
}B;


void FunA() {
  printf("%s\n", "Base A::fun()");
}

void FunB() {
  printf("%s\n", "Derived B::fun()");
}

int main() {
  A a;
  B b;

  a._f = FunA;
  b._b._f = FunB;

  A *pa = &a;
  pa->_f();
  pa = (A*)&b;
  pa->_f();
  return 0;
}
