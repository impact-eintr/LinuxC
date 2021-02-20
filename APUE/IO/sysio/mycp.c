#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc,char** argv)
{
    if (argc < 3){
        fprintf(stdout,"Usage...");
        exit(1);
    }

    int sfd = open(argv[1],O_RDONLY);
    if (sfd < 0){
        strerror(errno);
        exit(1);
    }

    int dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(dfd < 0){
        close(sfd);
        strerror(errno);
        exit(1);
    }

    const int SIZE = 8192;
    char buf[SIZE];
    int count = 0;

    while(1){
        int len = 0;//read的返回值
        int ret = 0;//write的返回值
        int pos = 0;//写截至的位置
        len = read(sfd,buf,SIZE);
        if (len < 0){
            strerror(errno);
            break;
        }
        if (len == 0){
            break;
        }
        //以防写入不足
        while(len > 0){
            ret = write(dfd,buf+pos,len);
            if (ret < 0){
                printf("%s\n",strerror(errno));
                exit(1);
            }
            pos += ret;
            len -= ret;
        }
        count++;
    }
    printf("%d\n",count);
    close(dfd);
    close(sfd);

    exit(0);
}

