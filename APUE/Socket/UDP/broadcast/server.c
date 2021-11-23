#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "proto.h"

#define SERVERPORT "2333"

int main()
{
  int sfd;
  struct msg_st *sbuf;
  struct sockaddr_in raddr;//remote addr

  sfd = socket(AF_INET,SOCK_DGRAM,0/*IPPROTO_UDP*/);
  if(sfd < 0){
    perror("socket()");
    exit(1);
  }
  //设置socket的属性
  int val = 1;
  if (setsockopt(sfd,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val)) < 0){
    perror("setsockopt()");
    exit(1);
  }//打开广播属性


  int pkglen = sizeof(struct msg_st)+strlen("Mike")+1;// 注意给'/0'留位置
  sbuf = malloc(pkglen);
  if (sbuf == NULL){
    perror("malloc()");
    exit(1);
  }

  char *name = "Mike";
  strcpy(sbuf->name,name);
  sbuf->math = htonl(rand()%100);//主机字节序转网络字节序
  sbuf->chinese = htonl(rand()%100);

  raddr.sin_family = AF_INET;
  raddr.sin_port = htons(atoi(SERVERPORT));
  inet_pton(AF_INET,"192.168.47.255",&raddr.sin_addr);

  while(1) {
    if(sendto(sfd,sbuf,pkglen,0,(void *)&raddr,sizeof(raddr)) < 0){
      perror("sendto()");
      exit(1);
    }

    puts("OK");
    sleep(1);
  }

  close(sfd);

  exit(0);
}
