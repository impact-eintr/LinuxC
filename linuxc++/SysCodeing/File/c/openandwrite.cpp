#include "File.h"
#include <cstdio>
#include <fcntl.h>

int main(){
    int fd = -1,fd1 = -1,i;
    ssize_t size = -1;
    char buf[BUFSIZE];
    char filename[] = "./test.txt";

    fd = open(filename,O_RDWR|O_CREAT|O_TRUNC);
    if(fd < 0){
        perror("open()");
        exit(1);
    }

    char text[] = "You can use the pkg.go.dev site to find published modules whose packages have functions you can use in your own code. Packages are published in modules -- like rsc.io/quote -- where others can use them. Modules are improved with new versions over time, and you can upgrade your code to use the improved versions.\n";
    if(write(fd,text,strlen(text)) < 0){
        perror("write()");
        exit(1);
    }

    const char *test = "???";
    off_t offset = lseek(fd,7,SEEK_SET);
    if(offset < 0){
        perror("lseek()");
        exit(1);
    }
    if(write(fd,test,strlen(test)) < 0){
        perror("write()");
        exit(1);
    }

    fd1 = open(filename,O_RDONLY);
    while(size){
        size = read(fd1,buf,BUFSIZE);
        if(size == -1){
            close(fd);
            perror("read()");
            exit(1);
        }else{
            if(size > 0){
                for(i = 0;i< size;i++)
                  printf("%c",buf[i]);
            }else{
                printf("EOF\n");
            }
        }
    }
    close(fd);
}
