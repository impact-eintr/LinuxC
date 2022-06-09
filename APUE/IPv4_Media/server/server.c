#include <arpa/inet.h>
#include <bits/getopt_core.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "../include/proto.h"
#include "medialib.h"
#include "server_conf.h"
#include "thr_channel.h"
#include "thr_list.h"

int serversd;
struct sockaddr_in sndaddr;
struct server_conf_st server_conf = {.rcvport = DEFAULT_RCVPORT,
                                     .media_dir = DEFAULT_MEDIADIR,
                                     .runmode = RUN_FOREGROUND,
                                     .ifname = DEFAULT_IF,
                                     .mgroup = DEFAULT_MGROUP};
static struct mlib_listentry_st *list;

static void print_help() {
  printf("-P    specify receive port\n");
  printf("-M    specify multicast group\n");
  printf("-F    specify foreground runmode \n");
  printf("-D    specify medialib location \n");
  printf("-I    specify net card\n");
  printf("-H    show help\n");
}

// 守护进程退出
static void daemon_exit(int s) {
  thr_list_destroy();
  thr_channel_destroyall();
  mlib_freechnlist(list);
  syslog(LOG_WARNING, "signal-%d caught, exit now.", s);
  closelog();
  exit(0);
}

// 脱离终端
static int daemonize() {
  pid_t pid;
  int fd;
  pid = fork();
  if (pid < 0) {
    // perror("fork()");
    syslog(LOG_ERR, "fork() failed:%s", strerror(errno));
    return -1;
  }
  if (pid > 0) // parent
    exit(0);
  fd = open("/dev/null", O_RDWR);
  if (fd < 0) {
    // perror("open()");
    syslog(LOG_ERR, "open() failed:%s", strerror(errno));
    return -2;
  } else {
    /*close stdin, stdout, stderr*/
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
      close(fd);
  }
  chdir("/");
  umask(0);
  setsid();
  return 0;
}

// socket 初始化通用步骤
static int socket_init() {
  struct ip_mreqn mreq;
  inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr);
  inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);      // local address
  mreq.imr_ifindex = if_nametoindex(server_conf.ifname); // net card
  serversd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serversd < 0) {
    syslog(LOG_ERR, "socket():%s", strerror(errno));
    exit(1);
  }
  if (setsockopt(serversd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) <
      0) {
    syslog(LOG_ERR, "setsockopt(IP_MULTICAST_IF):%s", strerror(errno));
    exit(1);
  }

  // bind
  sndaddr.sin_family = AF_INET;
  sndaddr.sin_port = htons(atoi(server_conf.rcvport));
  inet_pton(AF_INET, server_conf.mgroup, &sndaddr.sin_addr);
  // inet_pton(AF_INET, "0.0.0.0", &sndaddr.sin_addr.s_addr);

  return 0;
}

int main(int argc, char** argv) {
  int c;
  struct sigaction sa;

  sa.sa_handler = daemon_exit;
  sigemptyset(&sa.sa_mask);
  sigaddset(&sa.sa_mask, SIGINT);
  sigaddset(&sa.sa_mask, SIGQUIT);
  sigaddset(&sa.sa_mask, SIGTERM);

  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);

  openlog("netradio", LOG_PID | LOG_PERROR, LOG_DAEMON);
#ifdef DEBUG
  fprintf(stdout, "here1!\n");
#endif
  while(1) {
    c = getopt(argc, argv, "M:P:FD:I:H");
    printf("get command c:%c\n", c);
    if (c < 0) {
      break;
    }
    switch (c) {
    case 'M':
      server_conf.mgroup = optarg;
      break;
    case 'P':
      server_conf.rcvport = optarg;
      break;
    case 'F':
      break;
    case 'D':
      server_conf.media_dir = optarg;
      break;
    case 'I':
      server_conf.ifname = optarg;
      break;
    case 'H':
      print_help();
      exit(0);
      break;
    default:
      abort();
      break;
   }
  }

  if (server_conf.runmode == RUN_DAEMON) { // 后台运行
    if (daemonize() != 0) {
      perror("daemonize()");
    }
  } else if (server_conf.runmode == RUN_FOREGROUND) { // 前台运行

  } else {
    syslog(LOG_ERR, "EINVAL server_conf.runmode.");
    exit(1);
  }

  /*SOCKET initalize*/
  socket_init();
  /*get channel information*/
  int list_size;
  int err;

  // list 频道的描述信息
  // list_size有几个频道
  err = mlib_getchnlist(&list, &list_size);
  if (err) {
    syslog(LOG_ERR, "mlib_getchnlist():%s", strerror(errno));
    exit(1);
  }

  /*create programme thread*/
  thr_list_create(list, list_size);
  /*if error*/
  /*create channel thread*/
  //创建频道线程
  int i = 0;
  for (i = 0; i < list_size; i++) {
    err = thr_channel_create(list + i);
    if (err) {
      fprintf(stderr, "thr_channel_create():%s\n", strerror(err));
      exit(1);
    }
  }
  syslog(LOG_DEBUG, "%d channel threads created.", i);
  while (1)
    pause();
}
