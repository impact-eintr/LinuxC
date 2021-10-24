#include <stdlib.h>
#include <stdio.h>

void func() {
  static int x = 1;
  x++;
  printf("%d\n", x);
}

int main() {
  func();
  func();
  func();
}
