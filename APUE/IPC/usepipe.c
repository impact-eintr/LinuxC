#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <errno.h>

#define BUFSIZE 1024

int main()
{
    int pd[2];
    pid_t pid;
    char buf[BUFSIZE];

    if (pipe(pd) < 0){
        perror("pipe()");
        exit(1);
    }

    fprintf(stdout,"hello,welcome to MusicPlayer\n");

    pid = fork();
    if (pid < 0){
        perror("fork()");
        exit(1);
    }
    
    if(pid == 0){
        close(pd[1]); //关闭写端

        dup2(pd[0],0);//将将读端重定向到stdin mpg123 - "-" 表示从stdin读取
        close(pd[0]);

        int fd = open("/dev/null",O_RDWR);//屏蔽其他标准输出
        dup2(fd,1);
        dup2(fd,2);

        execl("/bin/mpg123","mpg123","-",NULL);
        perror("execvp()");
        exit(1);
    }else{
        close(pd[0]);//关闭读端
        int fd = open("./test.mp3",O_RDONLY);
        int len;

        while(1){
            len = read(fd,buf,BUFSIZE);
            if (len < 0){
                if (errno == EINTR)
                  continue;
                close(pd[1]);
                exit(1);
            }
            if (len == 0){
                break;
            }
            write(pd[1],buf,len);
        }
        close(pd[1]);
        wait(NULL);
    }

    exit(0);
}

