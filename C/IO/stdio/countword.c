#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*******
 *这节主要讲fgetc 可以用来统计文件的大小
 *
 * *****/

int main(int argc,char **argv)
{
    FILE *fp = NULL;
    int count = 0;

    if (argc < 2){
        fprintf(stderr,"Usage:\n");
        exit(1);
    }

    fp = fopen(argv[1],"r");
    if (fp == NULL){
        perror("fopen()");
        exit(1);
    }

    while(fgetc(fp) != EOF){
        count++;
    }
    printf("%d\n",count);
    fclose(fp);
    return 0;
}

