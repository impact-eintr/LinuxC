#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
	int fd = -1;
	ssize_t size = -1; 
	off_t offset = -1; 

	char buf[] = "boys";
	char filename[] = "/root/test.txt";
    
	fd = open(filename, O_RDWR);  
	if (-1 == fd)
	{   
		printf("Open file %s failure,fd:%d", filename, fd);
		return -1; 
	}   
	offset = lseek(fd, 5, SEEK_SET);  
	if (-1 == offset)
	{   
		printf("lseek file %s failure,fd:%d", filename, fd);
		return -1; 
	}   
	size = write(fd, buf, strlen(buf));  
	if (size != strlen(buf))
	{   
		printf("write file %s failure,fd:%d", filename, fd);
		return -1; 
	}   
	close(fd);
	return 0;
}
