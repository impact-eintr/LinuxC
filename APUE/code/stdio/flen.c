#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* fps;
    fps = fopen(argv[1], "r");
    if (fps == NULL) {
	perror("fopen()");
	exit(1);
    }
    fseek(fps, 0, SEEK_END);

    //ftell
    printf("%ld\n", ftell(fps));
}
