#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <strings.h>  
#include <unistd.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <dirent.h>  
#include <sys/mman.h>  
#include <sys/wait.h>  
#include <signal.h>  
#include <sys/ipc.h>  
#include <sys/shm.h>  
#include <sys/msg.h>  
#include <sys/sem.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <poll.h>  
#include <sys/epoll.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  

char wbuf[50];  
  
int main()  
{  
    int sockfd;  
    int size,on = 1;  
    struct sockaddr_in saddr;  
    int ret;  
  
    size = sizeof(struct sockaddr_in);  
    bzero(&saddr,size);  
  
    //设置地址信息，ip信息  
    saddr.sin_family = AF_INET;  
    saddr.sin_port = htons(8888);  
    saddr.sin_addr.s_addr = inet_addr("172.16.2.6");//172.16.2.6为服务端所在的ip  
    
    sockfd= socket(AF_INET,SOCK_DGRAM,0);  //创建udp 的套接字  
    if(sockfd<0)  
    {  
        perror("failed socket");  
        return -1;  
    }  
    //设置端口复用  
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));  
  
    //循环发送信息给服务端  
    while(1)  
    {  
        puts("please enter data:");  
        scanf("%s",wbuf);  
        ret=sendto(sockfd,wbuf,50,0,(struct sockaddr*)&saddr,  
            sizeof(struct sockaddr));  
        if(ret<0)  
        {  
            perror("sendto failed");  
        }  
  
        bzero(wbuf,50);  
    }  
    close(sockfd);  
    return 0;  
}  
