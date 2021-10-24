#include "stdio.h"
#include "stdlib.h"

extern int i;

void func() {
  printf("[%s]%d\n", __FUNCTION__,i);
}
