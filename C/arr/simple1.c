#include <stdio.h>
#include <stdlib.h>

int main() {
  int arr[3] = {1, 2, 3};
  printf("%ld\n", sizeof(arr));
  for (int i = 0;i < sizeof(arr)/sizeof(int);i++) {
    printf("%d ", *(arr+i));
  }
}
