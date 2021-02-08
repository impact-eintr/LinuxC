#include <apue.h>
#include <fcntl.h>

char buf1[] = "abc";
char buf2[] = "ABC";

int main(void)
{
    int fd;

    if ((fd = creat("file.helo", 0755)) < 0) {
	perror("creat error");
	exit(1);
    }

    if (write(fd, buf1, 3) != 3) {
	perror("write error");
	exit(1);
    }

    if (lseek(fd, 100, SEEK_SET) == -1) {
	perror("write error");
	exit(1);
    }

    if (write(fd, buf2, 3) != 3) {
	perror("write error");
	exit(1);
    }

    exit(0);
}
