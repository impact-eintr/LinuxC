#include "apue.h"

int main(int argc, char** argv)
{
    if (chdir(argv[1]) < 0)
	perror("chdir()");
    printf("succeed\n");
    exit(0);
}
