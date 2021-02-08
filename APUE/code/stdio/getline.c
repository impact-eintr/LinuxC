#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* fp;
    char* linebuf = NULL;
    size_t linesize = 0;

    if (argc < 2) {
	fprintf(stderr, "Usage...\n");
	exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
	perror("open error");
	exit(1);
    }

    while (1) {
	getline(&linebuf, &linesize, fp);
    }
}
