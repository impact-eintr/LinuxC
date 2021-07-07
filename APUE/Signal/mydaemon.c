#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>

#define FNAME "/tmp/out"

static FILE* fp;

static int deamonize(){
    int fd;
    pid_t pid;
    pid = fork();

    if (pid < 0){
        return -1;
    }

    if (pid > 0){
        exit(0);
    }

    fd = open("/dev/null",O_RDWR);//输出都忽略
    if (fd < 0){
        return -1;
    }
    if (pid == 0){
        fflush(NULL);
        dup2(fd,0);
        dup2(fd,1);
        dup2(fd,2);
        if (fd > 2){
            close(fd);
        }
        setsid();//脱离终端
        //umask();
        chdir("/");
    }
    return 0;
}

static void daemon_exit(int sig){
    syslog(LOG_WARNING,"服务即将关闭!");
    closelog();
    fclose(fp);
}

int main()
{
    struct sigaction sa;

    sa.sa_handler = daemon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGQUIT);
    sigaddset(&sa.sa_mask,SIGTERM);
    sigaddset(&sa.sa_mask,SIGINT);
    sa.sa_flags = 0;

    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);
    sigaction(SIGTERM,&sa,NULL);
    
    //开启日志服务
    openlog("print i",LOG_PID,LOG_DAEMON);

    if (deamonize()){
        syslog(LOG_ERR,"init failed!");
    }else{
        syslog(LOG_INFO,"successded!");
    }

    fp = fopen(FNAME,"w+");
    if (fp == NULL){
        syslog(LOG_ERR,"write file failed!");
        exit(1);
    }

    syslog(LOG_INFO,"%s opened",FNAME);

    for(int i = 0; ;i++){
        fprintf(fp,"%d\n",i);
        fflush(NULL);
        syslog(LOG_DEBUG,"%d 写入",i);
        sleep(1);
    }
    exit(0);
}

