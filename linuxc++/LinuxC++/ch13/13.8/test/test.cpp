#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>

#define BUFFER_SIZE 512



void u_alarm_handler(int n)
{
	printf("alarm:-----------connect timeout----------\n");
}

int main( int argc, char* argv[] )
{

	char ip[]="172.16.2.88"; //本机的ip是172.16.2.7，172.16.2.88并不存在
	int port = 13334;

	struct sockaddr_in server_address;
	bzero( &server_address, sizeof( server_address ) );
	server_address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &server_address.sin_addr );
	server_address.sin_port = htons( port );

	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );



	int old_option = fcntl( sock, F_GETFL );
	printf("noblock: %d\n", old_option & O_NONBLOCK); //0 is block mode


	struct  timeval tv1, tv2;
	gettimeofday(&tv1, NULL);


	sigset(SIGALRM, u_alarm_handler);
	alarm(1);//设置1秒超时
int ret = connect( sock, ( struct sockaddr* )&server_address, sizeof( server_address ) );
	alarm(0);
	sigrelse(SIGALRM);


	
	printf("connect ret code is: %d\n", ret);
	if (  ret == 0 )
	{
		printf("call getsockname ...\n");
		struct sockaddr_in local_address;
		socklen_t length;
		int ret = getpeername(sock, ( struct sockaddr* )&local_address, &length);
		assert(ret == 0);
		char local[INET_ADDRSTRLEN ];
		printf( "local with ip: %s and port: %d\n",
			inet_ntop( AF_INET, &local_address.sin_addr, local, INET_ADDRSTRLEN ), ntohs( local_address.sin_port ) );


		char buffer[ BUFFER_SIZE ];
		memset( buffer, 'a', BUFFER_SIZE );
		send( sock, buffer, BUFFER_SIZE, 0 );
	}
	else if (ret == -1)
	{
		gettimeofday(&tv2, NULL);
		suseconds_t msec = tv2.tv_usec - tv1.tv_usec;
		time_t sec = tv2.tv_sec - tv1.tv_sec;
		printf("time used:%d.%fs\n", sec, (double)msec / 1000000 );

		printf("connect failed...\n");
		if (errno == EINPROGRESS)
		{
			printf("unblock mode ret code...\n");
		}
	}
	else
	{
		printf("ret code is: %d\n", ret);
	}


	close( sock );
	return 0;
}
