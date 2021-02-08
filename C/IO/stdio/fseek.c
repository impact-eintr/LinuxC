#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    const int SIZE = 1024;
    FILE *fp = NULL;
    char buf[SIZE];
    buf[0] = 'A';
    buf[1] = 'A';
    buf[2] = 'A';
    buf[3] = 'A';

    fp = fopen("./tmp","w+");
    if (fp == NULL){
        strerror(errno);
        exit(1);
    }
    
    int i = 0;
    while(i < 10){
        unsigned long n = fwrite(buf,1,4,fp);
        fseek(fp,-n,SEEK_CUR);
        unsigned long len =  fread(buf,1,n,fp);
        printf("%lu\n",len);
        fseek(fp,0,SEEK_END);
        i++;
    }
    
    fseek(fp,1024,SEEK_CUR);
    return 0;
}

