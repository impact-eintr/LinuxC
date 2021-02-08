#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 1024

int main()
{
    int pd[2];
    pid_t pid;
    int fd;

    if (pipe(pd) < 0) {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }

    if (pid == 0) {
        close(pd[1]); //关写端
        dup2(pd[0], 0);
        close(pd[0]);
        fd = open("/dev/null", O_RDWR);
        dup2(fd, 1);
        dup2(fd, 2);
        execl("/usr/bin/mpg123", "mpg123", "-", "NULL");
        perror("execl()");
        exit(1);
    } else {
        close(pd[0]); //关读端
        //a socket come
        close(pd[1]);
        wait(NULL);
        exit(0);
    }

    exit(0);
}
