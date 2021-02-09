#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    int fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }

    long err = lseek(fd,5LL*1024LL*1024LL*1024LL-1LL,SEEK_SET);
    if (err == -1) {
        perror("lseek");
        exit(1);
    }

    write(fd,"",1);

    return 0;
}

