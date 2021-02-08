#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int i = 0;
	int fd = 0;
	for (i = 1; fd >= 0; i++)
	{   
		fd = open("test.txt", O_RDONLY);
		if (fd > 0)
			 printf("fd:%d\n", fd);
		else
		{   
			printf("error,can't openf file \n");
			exit(1);
		}   
	}   
	return 0;
}