#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "relay.h"

#define BUFSIZE 1024
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"

enum {
    STATE_R = 1,
    STATE_W,
    STATE_Ex,
    STATE_T
};

struct fsm_st{
    int state;
    int sfd;//源
    int dfd;//目标
    char buf[BUFSIZE];
    int len;
    int pos;
    //出错原因
    char *errMess;
};

int main()
{
    int fd1,fd2;
    int job1;

    //假设用户使用阻塞的方式打开设备
    fd1 = open(TTY1,O_RDWR);
    if (fd1 < 0){
        perror("open()");
        exit(1);
    }
    write(fd1,"TTY1\n",5);

    fd2 = open(TTY2,O_RDWR|O_NONBLOCK);
    if (fd2 < 0){
        perror("open()");
        close(fd1);
        exit(1);
    }
    write(fd2,"TTY2\n",5);

    job1 = relay_addjob(fd1,fd2);
    if (job1 < 0){
        fprintf(stderr,"relay_addjob() err :%s\n",strerror(-job1));
        exit(1);
    }
    while(1)
      pause();

    close(fd1);
    close(fd2);
    exit(0);
}

