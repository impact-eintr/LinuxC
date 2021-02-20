#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef false
#include <stdio.h>
FILE *fopen(const char *path,const char *mode);
FILE *fdopen(int fd,const char *mode);
FILE *freopen(const char *pathmconst char *mode,FILE *)
#endif

int main()
{
#ifdef false
    char *ptr = "abc";
    ptr[0] = 'x';如果要修改的话，会报段错误。意图修改一个常量
    printf("%s\n",ptr);
#endif

    FILE *fp;
    fp = fopen("log","r");
    if (fp == NULL){
        fprintf(stderr,"fopen() faild! errno = %d\n",errno);//errno已经被私有化
        perror("fopen");
        printf("%s\n",strerror(errno));
        exit(1);
    }else{
        fputs("ok!\n",stdout);
        fputs("OK\n",fp);
    }
    return 0;
}

