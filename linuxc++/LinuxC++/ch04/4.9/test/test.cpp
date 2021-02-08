#include <fcntl.h>
#include <stdio.h>
#include <error.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	struct flock lock;  
	int res, fd = open("myfile.txt", O_RDWR | O_CREAT, 0777);  
	if (fd > 0)
	{
		lock.l_type = F_WRLCK; 
		lock.l_whence = SEEK_SET;  
		lock.l_start = 0;  
		lock.l_len = 0;  
		lock.l_pid = getpid();
		res  = fcntl(fd, F_SETLK, &lock);  
		printf("return value of fcntl=%d\n", res);
		while (true)
			;
	}
	
	return 0;
}
