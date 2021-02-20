/****************
 *主要讲fgets fputs两个函数
 *
 *
 * **************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SIZE 5

int main()
{   
    FILE *fp = NULL;
    fp = fopen("./tmp","r");
    if (fp == NULL){
        perror("fopen()");
        exit(1);
    }
#ifdef false
    char buf[SIZE];

    while(fgets(buf,SIZE,fp)){
        printf("%s\n",buf);
    }
#endif
    fprintf(stdout,"生日快乐\n");
    fclose(fp);

    return 0;
}

