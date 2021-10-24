#include <stdlib.h>
#include <stdio.h>

#define MAX(a,b) ({int A=a,B=b; ((A) > (B) ? (A) : (B));})

int main() {
  int a = 3, b = 5;
  printf("%d\n",MAX(a++, b++));
  printf("%d\n",MAX(a++, b++));
}
