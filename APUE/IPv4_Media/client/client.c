#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
//#include <proto.h>
#include "../include/proto.h"
#include "client.h"
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <net/if.h>
#include <string.h>

/*
-M --mgroup specify multicast group
-P --port specify receive port
-p --player specify player
-H --help show help
*/

struct client_conf_st client_conf = {.rcvport = DEFAULT_RCVPORT,
                                     .mgroup = DEFAULT_MGROUP,
                                     .player_cmd = DEFAULT_PLAYERCMD};

static void print_help() {
  printf("-P --port   specify receive port\n");
  printf("-M --mgroup specify multicast group\n");
  printf("-p --player specify player \n");
  printf("-H --help   show help\n");
}

#define BUFSIZE 320*1024/8*3

/*write to fd len bytes data*/
static int writen(int fd, const void *buf, size_t len) {
  int count = 0;
  int pos = 0;
  while (len > 0) {
    count = write(fd, buf + pos, len);
    if (count < 0) {
      if (errno == EINTR)
        continue;
      perror("write()");
      return -1;
    }
    len -= count;
    pos += count;
  }
  return 0;
}

int main(int argc, char *argv[]) {

  /*
  initializing
  level:default < configuration file < environment < arg
  */
  int index = 0;
  int sd = 0;
  struct ip_mreqn mreq;     // group setting
  struct sockaddr_in laddr; // local address
  uint64_t receive_buf_size = BUFSIZE;
  int pd[2];
  pid_t pid;
  struct sockaddr_in server_addr;
  socklen_t serveraddr_len;
  int len;
  int chosenid;
  int ret = 0;
  struct msg_channel_st *msg_channel;
  struct sockaddr_in raddr;
  socklen_t raddr_len;

  struct option argarr[] = {{"port", 1, NULL, 'P'},
                            {"mgroup", 1, NULL, 'M'},
                            {"help", 0, NULL, 'H'},
                            {NULL, 0, NULL, 0}};
  int c;
  while (1) {
    /*long format argument parse*/
    c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);
    if (c < 0)
      break;
    switch (c) {
    case 'P':
      client_conf.rcvport = optarg;
      break;
    case 'M':
      client_conf.mgroup = optarg;
      break;
    case 'h':
      client_conf.player_cmd = optarg;
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

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("socket()");
    exit(0);
  }
  // multicast group
  inet_pton(AF_INET, client_conf.mgroup,
            &mreq.imr_multiaddr); // 255.255.255.255-->0xFF..
  // local address(self)
  inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
  // local net card
  mreq.imr_ifindex = if_nametoindex("enp4s0f3u2u2");
  if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt()");
    exit(1);
  }
  // improve efficiency
  if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &(receive_buf_size), sizeof(receive_buf_size)) < 0) {
    perror("setsockopt()");
    exit(1);
  }
  laddr.sin_family = AF_INET;
  laddr.sin_port = htons(atoi(client_conf.rcvport));
  inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
  if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
    perror("bind()");
    exit(1);
  }
  if (pipe(pd) < 0) {
    perror("pipe()");
    exit(1);
  }

  pid = fork();
  if (pid < 0) {
    perror("fork()");
    exit(1);
  }
  if (pid == 0) // child, read, close write
  {
    /*decode*/
    /*mpg123 read from stdin*/
    close(sd);      // socket
    close(pd[1]);   // 0:read, 1:write
    dup2(pd[0], 0); // set pd[0] as stdin
    if (pd[0] > 0)  // close pd[0]
      close(pd[0]);
    /*use shell to parse DEFAULT_PLAYERCMD, NULL means to end*/
    execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
    perror("execl()");
    exit(1);
  } else // parent
  {
    /*receive data from network, write it to pipe*/
    // receive programme
    struct msg_list_st *msg_list;
    msg_list = malloc(MSG_LIST_MAX);
    if (msg_list == NULL) {
      perror("malloc");
      exit(1);
    }
    //必须从节目单开始
    while (1) {
      len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&server_addr,
                     &serveraddr_len);
      fprintf(stderr, "server_addr:%d\n", server_addr.sin_addr.s_addr);
      if (len < sizeof(struct msg_list_st)) {
        fprintf(stderr, "massage is too short.\n");
        continue;
      }
      if (msg_list->chnid != LISTCHNID) {
        fprintf(stderr, "current chnid:%d.\n", msg_list->chnid);
        fprintf(stderr, "chnid is not match.\n");
        continue;
      }
      break;
    }

    // printf programme, select channel
    /*
    1.music xxx
    2.radio xxx
    3.....
    */
    // receive channel package, send it to child process
    struct msg_listentry_st *pos;
    for (pos = msg_list->entry; (char *)pos < ((char *)msg_list + len);
         pos = (void *)((char *)pos) + ntohs(pos->len)) {
      printf("channel:%d%s", pos->chnid, pos->desc);
    }
    /*free list*/
    free(msg_list);
    while (ret < 1) {
      ret = scanf("%d", &chosenid);
      if (ret != 1)
        exit(1);
    }

    msg_channel = malloc(MSG_CHANNEL_MAX);
    if (msg_channel == NULL) {
      perror("malloc");
      exit(1);
    }
    raddr_len = sizeof(raddr);
    char ipstr_raddr[30];
    char ipstr_server_addr[30];
    char rcvbuf[BUFSIZE];
    uint32_t offset = 0;
    memset(rcvbuf, 0, BUFSIZE);
    int bufct = 0; // buffer count
    while (1) {
      len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void *)&raddr, &raddr_len);
      //防止有人恶意发送不相关的包
      if (raddr.sin_addr.s_addr != server_addr.sin_addr.s_addr) {
        inet_ntop(AF_INET, &raddr.sin_addr.s_addr, ipstr_raddr, 30);
        inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, ipstr_server_addr, 30);
        fprintf(stderr, "Ignore:addr not match. raddr:%s server_addr:%s.\n",
                ipstr_raddr, ipstr_server_addr);
        continue;
      }
      if (raddr.sin_port != server_addr.sin_port) {
        fprintf(stderr, "Ignore:port not match.\n");
        continue;
      }
      if (len < sizeof(struct msg_channel_st)) {
        fprintf(stderr, "Ignore:massage too short.\n");
        continue;
      }

      if (msg_channel->chnid == chosenid) {
        memcpy(rcvbuf + offset, msg_channel->data, len - sizeof(chnid_t));
        offset += len - sizeof(chnid_t);

        if (bufct++ % 2 == 0) {
          if (writen(pd[1], rcvbuf, offset) < 0) {
            exit(1);
          }
          offset = 0;
        }
      }

      //可以做一个缓冲机制，停顿1  2 秒，不采用接收一点播放一点
      // if (msg_channel->chnid == chosenid) {
      //  if (writen(pd[1], msg_channel->data, len - sizeof(chnid_t)) < 0) {
      //    exit(1);
      //  }
      //}
    }

    free(msg_channel);
    close(sd);
    exit(0);
  }
}
