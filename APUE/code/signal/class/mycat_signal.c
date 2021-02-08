#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define CPS 10
#define BUFSIZE CPS
#define BURST 100

static volatile int token = 0;

static void alrm_handler(int s)
{
    token++;
    if (token > BURST)
	token = BURST;
}

int main(int argc, char* argv[])
{
    int sfd, dfd = 1; //源文件 目标文件
    char buf[BUFSIZ];
    int len, ret, pos;
    struct itimerval itv;

    if (argc < 2) {
	fprintf(stderr, "Usage...\n");
	exit(1);
    }

    signal(SIGALRM, alrm_handler);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, NULL);

    do {
	sfd = open(argv[1], O_RDONLY);
	if (sfd < 0) {
	    if (errno != EINTR) {
		perror("open()");
		exit(1);
	    }
	}
    } while (sfd < 0);

    while (1) {

	if (token <= 0)
	    pause();
	token--;

	while ((len = read(sfd, buf, BUFSIZE)) < 0) {

	    if (errno == EINTR)
		continue;
	    perror("read()");
	    break;
	}
	if (len == 0)
	    break;

	pos = 0;
	while (len > 0) {
	    ret = write(dfd, buf + pos, len);
	    if (ret < 0) {
		if (errno == EINTR)
		    continue;
		perror("write()");
		exit(1);
	    }
	    pos += ret;
	    len -= ret;
	}
	sleep(1);
    }
    close(sfd);

    exit(0);
}
