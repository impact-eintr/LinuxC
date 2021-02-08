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
	int sfp,nfp;
	struct sockaddr_in s_add,c_add;
	socklen_t sin_size;
	unsigned short portnum=10051;

	printf("Hello,I am a server,Welcome to connect me !\r\n");
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfp)
	{
		printf("socket fail ! \r\n");
		return -1;
	}
	printf("socket ok !\r\n");


	int on = 1;
	setsockopt( sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );//允许地址的立即重用


	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr=htonl(INADDR_ANY);
	s_add.sin_port=htons(portnum);

	if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
		printf("bind fail:%d!\r\n",errno);
		return -1;
	}
	printf("bind ok !\r\n");

	if(-1 == listen(sfp,5))
	{
		printf("listen fail !\r\n");
		return -1;
	}
	printf("listen ok\r\n");

	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);
		if(-1 == nfp)
		{
			printf("accept fail !\r\n");
			return -1;
		}
		printf("accept ok!\r\nServer start get connect from ip=%s,port=%d\r\n",inet_ntoa(c_add.sin_addr)  ,ntohs(c_add.sin_port));


		if(-1 == write(nfp,"hello,client,you are welcome! \r\n",32))
		{
			printf("write fail!\r\n");
			return -1;
		}
		printf("write ok!\r\n");
		close(nfp);

		puts("continue to listen(y/n)?");
		char ch[2];
		scanf("%s", ch, 2); //读控制台两个字符，包括回车符
		if (ch[0] != 'y') //如果不是y就退出循环
			break;


	}

	printf("bye!\n");
	close(sfp);
	return 0;
}
 

