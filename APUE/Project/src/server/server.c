#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "server_conf.h"
//#include <prote.h>
#include "../include/proto.h"
#include "medialib.h"
#include "thr_channel.h"
#include "thr_list.h"

/*
 *-M    指定多播组
 *-P    指定接收端口
 *-F    前台运行
 *-D    指定媒体库位置
 *-I    指定走哪个网卡
 *-h    显示帮助
 * */

struct server_conf_st server_conf = { .rcvport = DEFAULT_RCVPROT,
    .mgroup = DEFAULT_MGOUP,
    .media_dir = DEFAULT_MEDIADIR,
    .runmode = RUN_DAEMON,
    .ifname = DEFAULT_IF };

static void printhelp()
{
    printf("-M --mgroup 指定多播组\n-P --port 指定接收端口\n\
                -p --player 指定播放器\n-I --ifname 指定走哪个网卡\n\
                -D --media_dir 指定媒体库位置\n-h --help 显示帮助\n");
}

static void daemon_exit(int s) { }

static int daemonize()
{
    int fd;
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        return -1;
    }
    // parent
    if (pid > 0)
        exit(0);

    // child
    fd = open("/sev/null", O_RDWR);
    if (fd < 0) {
        syslog(LOG_ERR, "fork():%s", strerror(errno));
        return -2;
    }

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);

    if (fd > 2)
        close(fd);

    setsid();

    chdir("/data");
    umask(0);

    return 0;
}

//初始化socket
static int socket_init()
{
    int serversd;
    struct ip_mreqn mreq;

    serversd = socket(AF_INET, SOCK_DGRAM, 0);
    if (serversd < 0) {
        syslog(LOG_ERR, "socket():%s", strerror(errno));
        exit(1);
    }

    mreq.imr_ifindex = if_nametoindex(server_conf.ifname);
    inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);

    if (setsockopt(serversd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0) {
        syslog(LOG_ERR, "setsockopt(IP_MULTICAST_IF):%s", strerror(errno));
        exit(1);
    }

    return serversd;
}

int main(int argc, char** argv)
{
    int cmd;
    int sd;
    struct sigaction sa;
    sa.sa_handler = daemon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGTERM);

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    openlog("netredoio", LOG_PID | LOG_PERROR,
        LOG_DAEMON); //开启系统日志(守护进程)
    //命令行分析
    while (1) {
        cmd = getopt(argc, argv, "M:P:FD:I:h"); // getopt每次解析一个字符
        if (cmd < 0)
            break;
        switch (cmd) {
        case 'M':
            server_conf.mgroup = optarg;
            break;
        case 'P':
            server_conf.rcvport = optarg;
            break;
        case 'F':
            server_conf.runmode = RUN_FOREGROUND;
            break;
        case 'D':
            server_conf.media_dir = optarg;
            break;
        case 'I':
            server_conf.ifname = optarg;
            break;
        case 'h':
            printhelp();
            break;
        default:
            abort();
            break;
        }
    }

    //守护进程的实现
    if (server_conf.runmode == RUN_DAEMON) {
        if (daemonize() != 0) {
            exit(1);
        } else if (server_conf.runmode == RUN_FOREGROUND) {
            printf("正在前台运行\n");
        } else {
            syslog(LOG_ERR, "EINVAL server.conf.runmode");
            exit(1);
        }
    }
    // socket初始化
    sd = socket_init();

    //获取频道信息
    struct mlib_listyentry_st* list;
    int list_size = 100;
    int err;

    err = thr_list_create(list, list_size);
    if (err) {
    }

    //创建节目单线程
    for (int i = 0; i < list_size; i++) {
        thr_channel_create(list + i);
    }

    //创建频道线程

    while (1)
        pause();

    exit(0);
}
