#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int ftype(const char* filename)
{
    struct stat filestat;
    if (stat(filename, &filestat)) {
	perror("stat()");
	exit(1);
    }
    if (S_ISREG(filestat.st_mode))
	return '-';
    else if (S_ISDIR(filestat.st_mode))
	return 'd';
    else if (S_ISSOCK(filestat.st_mode))
	return 's';
    else if (S_ISLNK(filestat.st_mode))
	return 'l';
    else if (S_ISFIFO(filestat.st_mode))
	return 'f';
    else if (S_ISCHR(filestat.st_mode))
	return 'c';
    else if (S_ISBLK(filestat.st_mode))
	return 'b';
    else
	return '?';
}
int main(int argc, char* argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage ...");
	exit(1);
    }
    int filetype = 0;
    filetype = ftype(argv[1]);
    printf("%c\n", filetype);
}
