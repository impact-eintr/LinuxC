#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFSIZE 1024

int main(int argc, char* argv[])
{
    char buf[BUFSIZE];
    int fd1, fd2;
    fd1 = open(argv[1], O_RDONLY);
    lseek(fd1, 0, SEEK_SET);
    fd2 = open(argv[1], O_RDWR);
    lseek(fd2, 5, SEEK_SET);

    int n = 0;
    while ((n = read(fd1, buf, BUFSIZE)) > 0) {
	write(fd2, buf, n);
    }
    exit(1);
}
