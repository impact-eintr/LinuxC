#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> 

int main(void)
{
	int fds[2];
	if (pipe(fds) == -1) {
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		close(fds[0]);//子进程关闭读端
		sleep(10);
		write(fds[1], "hello", 5);
		exit(EXIT_SUCCESS);
	}

	close(fds[1]);//父进程关闭写端
	char buf[10] = { 0 };
	read(fds[0], buf, 10);
	printf("receive datas = %s\n", buf);
	return 0;
}