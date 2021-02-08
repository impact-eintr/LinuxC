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


int setnonblocking( int fd )
{
	int old_option = fcntl( fd, F_GETFL );
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}

int main( int argc, char* argv[] )
{
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );

	int old_option = fcntl( sock, F_GETFL );
	if(0==(old_option & O_NONBLOCK))
		printf("now socket is BLOCK mode\n"); //0 is block mode
	else 
		printf("now socket is NOT BLOCK mode\n"); 

	setnonblocking(sock);
	old_option = fcntl( sock, F_GETFL );
	if(0==(old_option & O_NONBLOCK))
		printf("now socket is BLOCK mode\n"); //0 is block mode
	else 
		printf("now socket is NOBLOCK mode\n"); 

	return 0;
}
