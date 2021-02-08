#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int fd = -1;
	char filename[] = "test.txt";
	fd = open(filename, O_CREAT | O_RDWR, S_IRWXU);
	if (fd == -1)
		printf("fail to pen file %s,fd:%d\n", filename, fd);
	else
		printf("Open file %s successfully,fd:%d\n", filename, fd);
	close(fd);
	return 0;
}