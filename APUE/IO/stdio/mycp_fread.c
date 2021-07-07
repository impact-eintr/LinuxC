#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 1024
/***************
 *fread 实现文件复制
 *
 **************/
int main()
{
    FILE *fps,*fpd;
    fps = fopen("./tmp","r");
    if (fps == NULL){
        strerror(errno);
        exit(1);
    }

    fpd = fopen("./copy","w");
    if (fpd == NULL){
        fclose(fps);
        strerror(errno);
        exit(1);
    }

    char buf[SIZE];

    int n = 0;
    while ((n = fread(buf,1,SIZE,fps)) > 0){
        printf("%d\n",n);
        fwrite(buf,1,n,fpd);
    }
    fclose(fps);
    fclose(fpd);
    return 0;
}

