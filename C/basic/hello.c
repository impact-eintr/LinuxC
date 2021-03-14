#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *num = malloc(sizeof(int));
    *num = 100;
    printf("%d\n",*num);
}

