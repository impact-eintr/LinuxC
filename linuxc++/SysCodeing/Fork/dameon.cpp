#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXFILE 65535

volatile sig_atomic_t _running = 1;

void sigterm_handler(int arg) { _running = 0; }

int main() {
    pid_t pc;
    int i, fd, len;
    const char *buf = "this is a Dameon\n";
    len = strlen(buf);
    pc = fork(); //创建子进程,父进程退出
    if (pc < 0) {
        printf("error fork\n");
        exit(1);
    } else if (pc > 0)
      exit(0);

    setsid();                     //在子进程中创建新对话
    chdir("/");                   //更改工作目录
    umask(0);                     //重设文件权限掩码,放开所有权限
    for (i = 0; i < MAXFILE; i++) //关闭文件描述符,当前进程的文件描述符,防止卸载文件系统失败
      close(i);
    signal(SIGTERM, sigterm_handler);
    while (_running) {
        if ((fd = open("/tmp/dameon.log", O_CREAT | O_WRONLY | O_APPEND, 0600)) <
                    0) {
            perror("open");
            exit(1);
        }
        write(fd, buf, len + 1);
        close(fd);
        usleep(10 * 1000); // 10ºÁÃë
    }
}
