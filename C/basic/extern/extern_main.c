#include <stdio.h>
#include <stdlib.h>
#include "extern.h"

int i = 10;

int main() {
  printf("[%s]%d\n", __FUNCTION__, i);
  func();
}
