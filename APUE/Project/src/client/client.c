#include <stdio.h>
#include <stdlib.h>

//#include "../include/proto.h"
#include "client.h"
#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <net/if.h>
#include <proto.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
 *-M --mgroup   指定多播组
 *-P --port     指定接收端口
 *-p --player   指定播放器
 *-h --help     显示帮助
 * */

//默认配置结构
//Linux内核喜欢用“.fieldname=value”的方式进行初始化
//使用指定初始化,一个明显的优点是成员初始化顺序和个数可变，并且扩展性好，
//比如增加字段时，避免了传统顺序初始化带来的大量修改。
struct client_conf_st client_conf = {
    .rcvport = DEFAULT_RCVPROT,
    .mgroup = DEFAULT_MGOUP,
    .player_cmd = DEFAULT_PLAYERCMD
};
static void printhelp()
{
    printf("-M --mgroup 指定多播组\n-P --port 指定接收端口\n\
                -p --player 指定播放器\n-h --help 显示帮助\n");
}

static ssize_t writen(int fd, const char* buf, size_t len)
{
    int ret;
    int pos = 0;

    while (len > 0) {
        ret = write(fd, buf + pos, len);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            perror("write()");
            return -1;
        }
        len -= ret;
        pos += ret;
    }
    return 0;
}

int main(int argc, char** argv)
{
    /*初始化
     * 级别 默认值 配置文件 命令行参数
     * */
    int pd[2]; //pipe管道一端读 一端写
    int index = 0;
    int cmd;
    int sd;
    pid_t pid;
    struct ip_mreqn mreq;
    struct sockaddr_in laddr, serveraddr, raddr;
    socklen_t serveraddr_len, raddr_len;
    struct option argarr[] = {
        { "port", 1, NULL, 'p' },
        { "mgroup", 1, NULL, 'M' },
        { "player", 1, NULL, 'p' },
        { "help", 0, NULL, 'h' },
        { NULL, 0, NULL, 0 }
    };

    while (1) {
        cmd = getopt_long(argc, argv, "P:M:p:", argarr, &index); //分析命令行
        if (cmd <= 0)
            break;

        switch (cmd) {
        case 'M':
            client_conf.mgroup = optarg;
            break;
        case 'P':
            client_conf.rcvport = optarg;
            break;
        case 'p':
            client_conf.player_cmd = optarg;
            break;
        case 'h':
            printhelp();
            exit(0);
        default:
            abort();
            break;
        }
    }

    //socket收数据
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }
    //点分式转大整数 协议 多播组地址 存放地址
    inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt()");
        exit(0);
    }

    int val = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val) < 0)) {
        perror("setsockopt()");
        exit(0);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);

    if (bind(sd, (void*)&laddr, sizeof(laddr)) < 0) {
        perror("bind()");
        exit(1);
    }

    //pipe管道机制实现通信
    if (pipe(pd)) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) {
        //child 调用解码器
        close(sd);
        close(pd[1]);   //关闭写
        dup2(pd[0], 0); //标准输入重定向读端
        if (pd[0] > 0)
            close(pd[0]);

        execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
        perror("execl()");
        exit(1);
    }

    //parent 从网络上接收包 发送给子进程
    //收节目单
    struct msg_list_st* msg_list;
    msg_list = malloc(MSG_LIST_MAX);
    if (msg_list == NULL) {
        perror("malloc()");
        exit(1);
    }

    int len;
    while (1) {
        len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void*)&serveraddr, &serveraddr_len);

        if (len < sizeof(struct msg_list_st)) {
            fprintf(stderr, "message is too small.\n");
            continue;
        }

        if (msg_list->chnid != LISTCHANNELNUMID) {
            fprintf(stderr, "CHANNELNUMID is not match(0).\n");
            continue;
        }
        break;
    }

    //打印节目单并选择频道
    struct msg_listentry_st* pos; //节目单类型的变量
    for (pos = msg_list->entry; (char*)pos < (((char*)msg_list) + len);
         pos = (void*)(((char*)msg_list) + ntohs(pos->len))) {
        printf("channel %d : %s \n", pos->chnid, pos->desc);
    }

    free(msg_list);

    int chosenid;
    int ret;
    while (1) {
        ret = scanf("%d", &chosenid);
        if (ret != 1) {
            fprintf(stderr, "输入有误\n");
            exit(1);
        }
    }
    //收频道包,发送给子进程
    struct msg_channel_st* msg_channel;

    msg_channel = malloc(MSG_CHANNEL_MAX);
    if (msg_channel == NULL) {
        perror("malloc()");
        exit(1);
    }

    while (1) {
        len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void*)&raddr, &raddr_len);
        if (raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr) {
            fprintf(stderr, "Ignore:address not match.\n");
            continue;
        }

        if (msg_channel->chnid == chosenid) {
            fprintf(stdout, "accepted msg:%d recieved\n", msg_channel->chnid);
            writen(pd[1], msg_channel->data, len - sizeof(chnid_t));
        }
    }

    free(msg_channel);
    close(sd);

    exit(0);
}
