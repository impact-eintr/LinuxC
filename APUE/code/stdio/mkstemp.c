#include "apue.h"
#include <asm-generic/errno-base.h>
#include <errno.h>

void make_temp(char* template);

int main(void)
{
    char good_template[] = "/tmp/dirXXXXXXX"; //正确的路径
    char* bad_template = "/tmp/dirXXXXXXX";

    printf("开始构建第一个临时文件..\n");
    make_temp(good_template);

    printf("开始构建第二个临时文件..\n");
    make_temp(bad_template);

    exit(0);
}

void make_temp(char* template)
{
    int fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) < 0) {
	close(fd);
	perror("cant't create temp file");
	exit(1);
    }
    printf("temp file name is %s\n", template);
    close(fd);
    if (stat(template, &sbuf) < 0) {
	if (errno == ENOENT)
	    printf("file does't exist\n");
	else {
	    perror("stat faild");
	    exit(1);
	}
    } else {
	printf("file exits\n");
	unlink(template);
    }
}
