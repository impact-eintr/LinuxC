#include <stdlib.h>
#include <stdio.h>

int main() {
  printf("[%s:%d] before while().", __FUNCTION__, __LINE__);
  fflush(stdout);
  while(1);
  printf("[%s:%d] after while().", __FUNCTION__, __LINE__);
}
