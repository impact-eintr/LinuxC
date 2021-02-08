#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#define FNAME "/tmp/out"

static FILE* fp;
static void daemon_exit(int s)
{
    fclose(fp);
    closelog();
}

static int daemonize(void)
{
    int fd;
    pid_t pid;
    struct sigaction sa;

#if 0
    signal(SIGINT, daemon_exit);
    signal(SIGQUIT, daemon_exit);
    signal(SIGTERM, daemon_exit);
#endif

    pid = fork();
    if (pid < 0)
	return -1;

    if (pid > 0) //父进程
	exit(0);

    if (pid > 0)
	exit(0);

    fd = open("/dev/null", O_RDWR);
    if (fd < 0)
	return -1;

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

    openlog("mydamon", LOG_PID, LOG_DAEMON);
    if (daemonize()) {
	syslog(LOG_ERR, "daemonnize() failed!");
	exit(1);
    } else {
	syslog(LOG_INFO, "daemonnize() OK!");
    }

    if (daemonize())
	exit(1);
    fp = fopen(FNAME, "w");
    if (fp == NULL) {
	syslog(LOG_ERR, "fopen() :%s", strerror(errno));
	exit(1);
    }

    for (int i = 0;; i++) {
	fprintf(fp, "已运行 %d s\n", i);
	fflush(fp);
	syslog(LOG_DEBUG, "%d is printed,", i);
	sleep(1);
    }

    exit(0);
}
