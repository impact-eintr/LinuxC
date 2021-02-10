#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BUFSIZE 1024

int main()
{

    char fmttime[BUFSIZ];
    int count = 0;

    FILE *fptr = fopen("./log","a+");
    if (fptr == NULL) {
        perror("fopen()");
        exit(1);
    }

    char buf[BUFSIZE];

    while(fgets(buf,BUFSIZE,fptr) != NULL){
        count++;
    }

    char res[BUFSIZE];

    while (1){
        time_t stamp;
        stamp = time(NULL);

        struct tm *struct_tm;
        struct_tm = localtime(&stamp);

        strftime(fmttime,BUFSIZE,"%Y-%m-%d %H:%M:%S",struct_tm);
        fprintf(fptr,"%d %s\n",++count,fmttime);
        fflush(fptr);
        sleep(1);
    }

    fclose(fptr);

    exit(0);
}

