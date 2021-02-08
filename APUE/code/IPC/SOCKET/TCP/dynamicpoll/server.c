#include "proto.h"
#include <asm-generic/socket.h>
#include <bits/types/sigset_t.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <time.h>

#define IPSIZE 40
#define BUFSIZE 1024
#define MINSPACESERVER 5
#define MAXSPACESERVER 10
#define MAXCLIENTS 15
#define SIG_NOTIFY SIGUSR2
#define LINEBUFSIZE 1024
#define IPSTRSIZE 40

enum {
    STATE_IDEL = 0,
    STATE_BUSY
};

struct server_st {
    pid_t pid;
    int state;
    int reuse; //进程留存计数
};

static struct server_st* serverpool;
static int idle_count = 0, busy_count = 0;
static int sd;

static void usr2_handler(int s)
{
    return;
}

static void server_job(int pos)
{
    int ppid; //父进程
    struct sockaddr_in raddr;
    socklen_t raddr_len;
    int client_sd;
    time_t stamp;
    int len;
    char linebuf[LINEBUFSIZE];
    char ipstr[IPSTRSIZE];

    ppid = getppid();
    raddr_len = sizeof(raddr);
    while (1) {
        serverpool[pos].state = STATE_IDEL;
        kill(ppid, SIG_NOTIFY);
        //raddr_len = sizeof(raddr);
        client_sd = accept(sd, (void*)&raddr, &raddr_len);
        if (client_sd < 0) {
            if (errno != EINTR || errno != EAGAIN) {
                printf("errno:%d\n", errno);
                perror("accept()");
                exit(1);
            }
        }

        serverpool[pos].state = STATE_BUSY;
        kill(ppid, SIG_NOTIFY);
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        //printf("[%d]client:%s:%d\n", getpid(), ipstr, ntohs(raddr.sin_port));
        printf("server_job idle_count%d", idle_count);

        stamp = time(NULL);
        len = snprintf(linebuf, LINEBUFSIZE, FMT_STAMP, (long long)stamp);
        send(client_sd, linebuf, len, 0);
        close(client_sd);
    }
}

static int add_one_server(void)
{
    int slot;
    pid_t pid;

    if (idle_count + busy_count >= MAXCLIENTS)
        return -1;
    for (slot = 0; slot < MAXCLIENTS; slot++)
        if (serverpool[slot].pid == -1)
            break;

    serverpool[slot].state = STATE_IDEL; //空闲态
    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) {
        server_job(slot);
        printf("slot = %d\n", slot);
        exit(0);
    } else {
        serverpool[slot].pid = pid;
        idle_count++;
        printf("add_one_server():->idle_count = %d\n", idle_count);
    }
    return 0;
}

static int del_one_server()
{
    if (idle_count == 0)
        return -1;

    for (int i = 0; i < MAXCLIENTS; i++) {
        if (serverpool[i].pid != -1 && serverpool[i].state == STATE_IDEL) {
            kill(serverpool[i].pid, SIGTERM);
            serverpool[i].pid = -1;
            idle_count--;
            break;
        }
    }
    return 0;
}

static int idle, busy = 0;
static int scanf_pool()
{
    printf("before idle = %d and busy = %d\n", idle, busy);
    for (int i = 0; i < MAXCLIENTS; i++) {
        if (serverpool[i].pid == -1) {
            printf("继续1\n");
            continue;
        }
        if (kill(serverpool[i].pid, 0)) {
            serverpool[i].pid = -1;
            printf("继续2\n");
            continue;
        }
        if (serverpool[i].state == STATE_IDEL) {
            idle++;
            printf("idle = %d\n", idle);
        } else if (serverpool[i].state == STATE_BUSY)
            busy++;
        else {
            fprintf(stderr, "Unknow state.\n");
            abort();
        }
    }
    printf("idle_count = %d and busy_count = %d\n", idle_count, busy_count);
    idle_count = idle;
    busy_count = busy;
    printf("after idle = %d and busy = %d\n", idle, busy);

    return 0;
}

int main()
{
    struct sigaction sa, osa;
    struct sockaddr_in laddr;
    sigset_t set, oset;
    int i;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;    //自行消亡
    sigaction(SIGCHLD, &sa, &osa); //保存状态

    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; //无特殊要求
    sigaction(SIG_NOTIFY, &sa, &osa);

    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &oset);

    serverpool = mmap(NULL, sizeof(struct server_st) * MAXCLIENTS,
        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (serverpool == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }

    for (int i = 0; i < MAXCLIENTS; i++)
        serverpool[i].pid = -1;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        perror("setsockopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);

    if (bind(sd, (void*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(sd, 100) < 0) {
        perror("listen()");
        exit(1);
    }

    //创建server端
    for (int i = 0; i < MINSPACESERVER; i++) {
        add_one_server();
    }

    printf("init idle_count:%d\n", idle_count);
    while (1) {
        sigsuspend(&oset);

        //scan_pool
        scanf_pool();

        //contrl the pool
        printf("%d\n", idle_count);
        if (idle_count > MAXSPACESERVER) { //太多了
            for (i = 0; i < (idle_count - MAXSPACESERVER); i++) {
                //sleep(1);
                //del_one_server();
            }
            printf("test:%d\n", idle_count);
        } else if (idle_count < MINSPACESERVER) { //太少了
            for (i = 0; i < (MINSPACESERVER - idle_count); i++)
                if (add_one_server() == -1) {
                    printf("???\n");
                }
        }
        //print pool status
        printf("输出状态\n");
        for (int i = 0; i < MAXCLIENTS; i++) {
            if (serverpool[i].pid == -1)
                putchar('o');
            else if (serverpool[i].state == STATE_IDEL)
                putchar('.');
            else
                putchar('x');
        }
        puts("\n");
    }

    sigprocmask(SIG_SETMASK, &oset, NULL);

    exit(0);
}
