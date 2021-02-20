#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *fp;
    fp = fopen("./tmp","r");
    if (fp == NULL){
        strerror(errno);
        exit(1);
    }


    return 0;
}

