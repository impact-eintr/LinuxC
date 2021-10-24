#include <stdio.h>
#include <stdlib.h>

int main() {
  int i = 0;
  char c = 0;

  scanf("%d", &i);
  //scanf("%*c%c", &c);
  // 或者
  getchar();
  scanf("%c", &c);
  printf("i = %d, c = %c", i, c);

}
