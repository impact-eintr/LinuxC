#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024
int main(int argc, char* argv[])
{
    FILE *fps, *fpd; //源文件 目标文件
    char buf[BUFSIZ];

    fps = fopen(argv[1], "r");
    if (fps == NULL) {
	perror("fopen()");
	exit(1);
    }

    fpd = fopen(argv[2], "w");
    if (fpd == NULL) {
	fclose(fps);
	perror("fopen()");
	exit(1);
    }

    //字符
    /*
    while (1) {
	int ch = fgetc(fps);
	if (ch == EOF) {
	    break;
	}
	fputc(ch, fpd);
    }
	*/
    //字符串
    while (fgets(buf, BUFSIZE, fps) != NULL)
	fputs(buf, fpd);

    fclose(fps);
    fclose(fpd);
    exit(0);
}
