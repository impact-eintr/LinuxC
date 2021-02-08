#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "unistd.h"
#include "errno.h"
#include <arpa/inet.h> //for inet_ntoa
  
int main()
{
	int sfp, nfp;
	struct sockaddr_in s_add, c_add;
	socklen_t sin_size;
	unsigned short portnum = 10051;
	struct sockaddr_in serv; 
	socklen_t serv_len = sizeof(serv); 
 
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sfp)
	{
		printf("socket fail ! \r\n");
		return -1;
	}
	printf("socket ok !\r\n");
	printf("ip=%s,port=%d\r\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port)); 

	int on = 1;
	setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));//允许地址的立即重用


	bzero(&s_add, sizeof(struct sockaddr_in));
	s_add.sin_family = AF_INET;
	s_add.sin_addr.s_addr =   inet_addr("192.168.0.2");
	s_add.sin_port = htons(portnum);

	if (-1 == bind(sfp, (struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
		printf("bind fail:%d!\r\n", errno);
		return -1;
	}
	printf("bind ok !\r\n");
	getsockname(sfp, (struct sockaddr *)&serv, &serv_len); 
	
	printf("ip=%s,port=%d\r\n", inet_ntoa(serv.sin_addr), ntohs(serv.sin_port));
	return 0;
}