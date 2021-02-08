#include "File.h"
#include <fcntl.h>

int main(){
    struct flock lock;
    int res,fd = open("test.txt",O_RDWR|O_CREAT,0777);
    if(fd > 0){
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start =0;
        lock.l_len = 0;
        lock.l_pid = getpid();
        res = fcntl(fd,F_SETLK,&lock);
        printf("return value of fcntl = %d\n",res);
        while(true);
    }
}
