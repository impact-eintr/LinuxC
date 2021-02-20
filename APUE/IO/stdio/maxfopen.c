#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    int count = 0;
    FILE *fp = NULL;

    while(1)
    {
        fp = fopen("tmp","r");
        if (fp == NULL)
        {
            perror("fopen()");
            break;
        }
        count++;
    }
    printf("%d\n",count);
    return 0;
}

