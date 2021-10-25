#include <stdlib.h>
#include <stdio.h>

int main () {
  int *p = (int[3]){1, 2, 3};
  for (int i = 0;i < 3;i++) {
    printf("%p->%d\n", &p[i], p[i]);
  }
}
