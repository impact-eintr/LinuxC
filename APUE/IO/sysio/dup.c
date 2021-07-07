#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FNAME "/tmp/out"


int main()
{
    int fd;
    fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }
    //dup 不原子
    close(1);//关闭标准输出
    dup(fd);
    close(fd);
    //dup2 原子
    //dup2(fd,1);//关闭1 重定向到fd

    //if (fd != 1){//如果fd本身是1 dup2 do nothing 之后无需关闭fd
    //    printf("%d\n",fd);
    //    close(fd);
    //}

    /*********************/
    printf("Hello world\n");
    return 0;
}

