#include "File.h"
#include <fcntl.h>

int main(){
    int fd = -1,i;
    ssize_t size = -1;
    char buf[10];
    char filename[] = "./test.txt";

    fd = open(filename,O_RDONLY);
    if(fd < 0){
        perror("open()");
        exit(1);
    }

    while(size){
        size = read(fd,buf,10);
        if(size == -1){
            close(fd);
            perror("read()");
            exit(1);
        }else{
            if(size > 0){
                printf("%ld bytes\n",size);
                for(i = 0;i< size;i++)
                  printf("%c",buf[i]);
                printf("\n");
            }else{
                printf("EOF\n");
            }
        }
    }
}
