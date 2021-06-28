#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("hello\n");
    int *num = malloc(sizeof(int));
    *num = 100;
    printf("%d\n",*num);
}

