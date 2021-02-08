#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void sys_err(const char *str){
    perror(str);
    exit(1);
}
//父写子读
int main(){
    pid_t pid;
    char buf[1024];
    int fd[2];//pipe
    char p[] = "test for pipe\n";

    if(pipe(fd) == -1)
      sys_err("pipe");

    pid = fork();
    if(pid < 0){
        sys_err("fork err");
    }
    else if(pid == 0){
        close(fd[1]);
        int len = read(fd[0],buf,sizeof(buf));
        sleep(100);
        write(1,buf,len);
        close(fd[0]);
    }else{
        close(fd[0]);
        write(fd[1],p,strlen(p));
        wait(NULL);
        close(fd[1]);
    }
    exit(0);
}
