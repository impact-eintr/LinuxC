#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE 4096
#define PATH "./log"

int main(void)
{

    FILE* fp;
    char buf[BUFSIZE];
    fp = fopen(PATH, "r+");

    time_t stmp;
    struct tm* tm;
    int count = 0;

    while (fgets(buf, BUFSIZE, fp) != NULL)
	count++;

    while (1) {
	time(&stmp);
	tm = localtime(&stmp);
	fprintf(fp, "%-4d%d-%d-%d %d:%d:%d\n", ++count,
	    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec);
	fflush(fp);
	sleep(1);
    }
    fclose(fp);
}
