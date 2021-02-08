#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int fd = -1;
	char filename[] = "/root/test.txt";
	fd = creat(filename,0666);
	if (fd == -1)
		printf("fail to pen file %s\n", filename);
	else
		printf("create file %s successfully\n", filename);
	 
	return 0;
}