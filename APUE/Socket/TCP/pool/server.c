#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>

#include "proto.h"

#define SIG_NOTIFY SIGUSR2

#define MINSPACESERVER 5
#define MAXSPACESERVER 10
#define MAXCLINETS     20

#define IPSIZE 1024
#define BUFSIZE 1024

enum {
    STATE_IDLE=0,  // idle 空闲的
    STATE_BUSY
};

struct server_st{
    pid_t pid;
    int state;
};

static struct server_st *serverpool;
static int idle_count = 0,busy_count = 0;

// socket相关全局变量
static int sfd;

static void handle(int sig){
    return;
}

static void server_job(int pos){
    int newsd;
    int ppid;

    struct sockaddr_in raddr;//remote addr
    char ip[IPSIZE];

    socklen_t raddr_len = sizeof(raddr);

    ppid = getppid();//父进程

    while(1) {
        serverpool[pos].state = STATE_IDLE;
        kill(ppid, SIG_NOTIFY);

        newsd = accept(sfd, (void *)&raddr,&raddr_len);//接收客户端连接
        if (newsd < 0){
            if (errno == EINTR || errno == EAGAIN)
              continue;
            else{
                perror("accept()");
                exit(1);
            }
        }

        serverpool[pos].state = STATE_BUSY;
        kill(ppid,SIG_NOTIFY);
        inet_ntop(AF_INET,&raddr.sin_addr,ip,IPSIZE);

        char buf[BUFSIZE];
        int pkglen = 0;

        pkglen = sprintf(buf,FMT_STAMP,(long long)time(NULL));

        if (send(newsd,buf,pkglen,0) < 0){
            perror("send()");
            exit(1);
        }
        close(newsd);
        sleep(5);
    }
}

static int add_one_server(){
    int slot;
    pid_t pid;

    if (idle_count + busy_count >= MAXCLINETS){
        return -1;
    }

    for(slot = 0;slot < MAXCLINETS;slot++){
        if (serverpool[slot].pid == -1){
            break;
        }
    }
    serverpool[slot].state = STATE_IDLE;
    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        server_job(slot);
        exit(0);
    }else{
        serverpool[slot].pid = pid;
        idle_count++;
    }
    return 0;
}

static int del_one_server(){
    int slot;
    if (idle_count == 0){
        return -1;
    }

    for(slot = 0;slot < MAXCLINETS;slot++) {
        if (serverpool[slot].pid != -1 && serverpool[slot].state == STATE_IDLE){
            kill(serverpool[slot].pid,SIGTERM);
            serverpool[slot].pid = -1;
            idle_count--;
            break;
        }
    }
    return 0;
}

static void scan_pool(){
    int idle = 0,busy = 0;
    for (int i = 0;i < MAXCLINETS;i++){
        if (serverpool[i].pid == -1){
            continue;
        }
        if (kill(serverpool[i].pid,0)){//kill pid 0检测一个进程是否存在
            serverpool[i].pid =-1;
            continue;
        }
        //统计进程池的状态
        if(serverpool[i].state == STATE_IDLE)
          idle++;
        else if(serverpool[i].state == STATE_BUSY)
          busy++;
        else{
            fprintf(stderr,"未知状态!\n");
            abort();
        }
    }
    idle_count = idle;
    busy_count = busy;
}

int main()
{
    struct sigaction sa,osa;
    
    sa.sa_handler = SIG_IGN;//忽略父进程的资源回收信号
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;//让子进程结束后自行消亡,不会变成僵尸状态
    sigaction(SIGCLD,&sa,&osa);
    
    sa.sa_handler = handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY,&sa,&osa);

    //屏蔽信号
    sigset_t sigset,oldsigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIG_NOTIFY);
    sigprocmask(SIG_BLOCK,&sigset,&oldsigset);

    serverpool = mmap(NULL,sizeof(struct server_st)*MAXCLINETS,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    if (serverpool == MAP_FAILED){
        perror("mmap()");
        exit(1);
    }

    //初始化进程池
    for (int i = 0;i < MAXCLINETS;i++){
        serverpool[i].pid = -1;
    }

    sfd = socket(AF_INET,SOCK_STREAM,0/*IPPROTO_TCP*/);
    if (sfd < 0){
        perror("socket()");
        exit(1);
    }
    
    int val = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0){
        perror("setsockopt()");
        exit(1);
    }

    struct sockaddr_in laddr;//local addr
    laddr.sin_family = AF_INET;//指定协议
    laddr.sin_port = htons(atoi(SERVERPORT));//指定网络通信端口
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);//IPv4点分式转二进制数

    if(bind(sfd,(void *)&laddr,sizeof(laddr)) < 0){
        perror("bind()");
        exit(1);
    }

    if(listen(sfd,1024) < 0){//全连接数量
        perror("listen()");
        exit(1);
    }

    for(int i = 0;i < MINSPACESERVER;i++){
        add_one_server();
    }

    while(1){
        sigsuspend(&oldsigset);

        scan_pool();//扫描进程池

        //管理进程池
        if (idle_count > MAXSPACESERVER){
            for (int i = 0;i < (idle_count-MAXSPACESERVER);i++){
                del_one_server();
            }
        }else if (idle_count < MINSPACESERVER){
            printf("削减server\n");
            for (int i = 0;i < (MINSPACESERVER-idle_count);i++){
                add_one_server();
            }
        }
        
        //print the pool
        for(int i = 0;i < MAXCLINETS;i++){
            if (serverpool[i].pid == -1){
                printf(" ");
            }else if (serverpool[i].state == STATE_IDLE){
                printf(".");
            }else if(serverpool[i].state == STATE_BUSY){
                printf("X");
            }
            fflush(NULL);
        }
        printf("\n");
    }

    close(sfd);
    sigprocmask(SIG_SETMASK,&oldsigset,NULL);
    
    exit(0);
}

