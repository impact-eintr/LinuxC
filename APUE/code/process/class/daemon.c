#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FNAME "/tmp/out"

static int daemonize(void)
{
    int fd;
    pid_t pid;

    pid = fork();
    if (pid < 0) {
	perror("fork()");
	return -1;
    }
    if (pid > 0) //父进程
	exit(0);

    if (pid > 0)
	exit(0);

    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
	perror("open()");
	return -1;
    }

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2) {
	close(fd);
    }

    setsid();

    chdir("/");
    return 0;
}

int main(void)
{
    FILE* fp;

    if (daemonize())
	exit(1);
    fp = fopen(FNAME, "w");
    if (fp == NULL) {
	perror("fopen()");
	exit(1);
    }

    for (int i = 0;; i++) {
	fprintf(fp, "已运行 %d s\n", i);
	fflush(fp);
	sleep(1);
    }
    exit(0);
}
