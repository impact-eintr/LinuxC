#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
//统计文件行数
int main()
{
    FILE *fp = NULL;
    fp = fopen("./tmp","r");
    if (fp == NULL){
        strerror(errno);
        exit(1);
    }

    size_t linesize = 0;
    char *line = NULL;

    while(1){
        if (getline(&line,&linesize,fp) < 0){
            break;
        }
        printf("%ld\n",strlen(line));
        printf("%ld\n",linesize);
    }
    return 0;
}

