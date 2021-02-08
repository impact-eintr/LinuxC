#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int flen(const char* filename)
{
    struct stat filestat;
    if (stat(filename, &filestat)) {
	perror("stat()");
	exit(1);
    }
    return filestat.st_size;
}

int main(int argc, char* argv[])
{
    int filelen = 0;
    filelen = flen(argv[1]);
    printf("%d", filelen);
}
