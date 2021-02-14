# 进程


### PID
类型 pid_t
- ps axf 查看进程树
- ps axm
- ps ax -L
- ps -ef
### 进程的产生
- fork() 
        - 注意理解关键字 duplicating 意味着拷贝 克隆 一模一样
        - fork 后父子进程的区别 ： fork 的返回值不一样 pid不同 ppid也不同 未决信号与文🔓 不继承资源利用量清0
        - init进程 是所以进程的祖先进程 pid == 1
        - 调度器的调度策略来决定哪个进程先运行
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("%d start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin

    pid_t pid = fork();

    if (pid == 0){
        printf("child %d\n",getpid());
    }else{
        printf("parent %d\n",getpid());
    }
    getchar();
    printf("pid %d end\n",getpid());
    return 0;
}

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define LEFT 2
#define RIGHT 200

int main()
{
    pid_t pid = 0;
    int i,j,mark;

    for (i = LEFT;i <= RIGHT;i++){
        pid = fork();
        if (pid == 0){
            mark = 1;
            for (j = 2;j < i/2;j++){
                if (i%j == 0){
                    mark = 0;
                    break;
                }
            }
            if (mark) {
                printf("%d is a primer\n",i);
            }
            exit(0);
        }
    }
    getchar();

    exit(0);
}

~~~

- vfork()

### 进程的消亡以及释放资源
- wait()
- waitpid()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define N 3
#define LEFT 100000002
#define RIGHT 100000200

//交叉算法计算 池类算法涉及到竞争
int main()
{
    printf("[%d] start !\n",getpid());
    fflush(NULL);//记得刷新 否则begin放到缓冲区 父子进程的缓冲区里各有一句begin
    pid_t pid = 0;
    int i,j,mark;

    for (int n = 0;n < N;n++){
        pid = fork();
        if (pid < 0){
            perror("fork");
            for (int k = 0;k < n;k++){
                wait(NULL);
            }
            exit(1);
        }

        if (pid == 0){
            for (i = LEFT+n;i <= RIGHT;i+=N){
                mark = 1;
                for (j = 2;j <= i/2;j++){
                    if (i%j == 0){
                        mark = 0;
                        break;
                    }
                }
                if (mark) {
                    printf("%d is a primer\n",i);
                }
            }
            printf("[%d] exit\n",n);
            exit(0);
        }

    }

    int st,n;
    for (n =0 ;n < N;n++){
        wait(&st);
        printf("%d end\n",st);
    }

    exit(0);
}

~~~

### exec函数族
**exec 替换 当前进程映像**
- `extern char **environ`
- execl
- execlp
- execle
- execv
- execvpa
###### 一个小shell
~~~ c
//lhq yyds
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <glob.h>
#include <string.h>

#define BUFSIZE 1024
#define DELIMS " \t\n"

extern char **environ;

static int cd(char *path){
    int ret = chdir(path);
    if (ret == -1){
        perror("chdir");
    }
    return ret;
}

static void readrc(char *name){
    FILE *fp;
    fp = fopen(name,"r+");
    //处理文件内容
    fclose(fp);
}

static void prompt()
{
    char pwd[BUFSIZE];
    char name[BUFSIZE];
    getcwd(pwd,BUFSIZE);
    getlogin_r(name,BUFSIZE);
    printf("%s %s $ ",name,pwd);
}

static int parse(char *linebuf,glob_t *globres){
    char *tok;
    int flag = 0;
    

    while (1){
        tok = strsep(&linebuf,DELIMS);
        if (tok == NULL){
            break;
            return -1;
        }else if(strcmp(tok,"cd") == 0){
            char *path = strsep(&linebuf,DELIMS);
            return cd(path);
        }else if(tok[0] == '\0'){
            continue;
        }

        glob(tok,GLOB_NOCHECK|GLOB_APPEND*flag,NULL,globres);//第一次不能append glob_argv中是随机值 GLOB_NOCHECK | (GLOB_APPEND*flag)==0 第一次不append
        flag = 1;
    }
    return 1;
}


//之后记得 将 ctrl+c 转为 stdout:\n 将ctrl+d 转为 退出+再见标语
int main()
{
    printf("This is YSHELL\n");

    pid_t pid;
    char *linebuf = NULL;
    size_t lienbuf_size = 0;
    glob_t globres;//解析命令行

    //读取配置文件
    char *yshrc = "/home/yixingwei/.yshrc";//填一个绝对路径
    readrc(yshrc);

    while(1){
        prompt();


        //获取命令
        getline(&linebuf,&lienbuf_size,stdin);
        //解析命令
        int ret = parse(linebuf,&globres);
        
        if (ret == -1){
            
        }else if (ret == 0){//内部命令
            
        }else if (ret == 1){//外部命令
            fflush(NULL);
            pid = fork();
            if (pid < 0){
                perror("fork()");
                exit(1);
            }else if(pid == 0){
                execvp(globres.gl_pathv[0],globres.gl_pathv);
                perror("execl()");
                exit(1);
            }
        }
        waitpid(pid,NULL,0);
    } 
    
    exit(0);
}

~~~

### 用户权限以及组权限
- u+s 当其他用户调用该可执行文件时，会切换成当前可执行文件的user的身份来执行
- g+s

- uid/gid
    - r real
    - e effective

函数
- getuid 返回 real
- geteuid 返回 effective
~~~ c
// mysudo
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc,char **argv)
{
    if (argc < 3){
        fprintf(stderr,"Useage");
        exit(1);
    }

    pid_t pid;

    pid = fork();
    if (pid == 0){
        setuid(atoi(argv[1]));
        execvp(argv[2],argv+2);
        perror("execvp()");
        exit(1);

    }else {
        wait(NULL);
    }

    exit(0);
}

~~~

~~~ bash
$ su
# chown root ./mysudo
# chmod u+s ./mysudo

$ ./mysudo 0 /etc/shadow
~~~
- getgid
- getegid
- setuid 设置effective
- setgid 设置effective
- setreuid 交换 r e //是原子的交换
- setregid 交换 r e

### system()
可以理解成 fork+exec+wait 封装

### 进程会计
- acct()

### 进程时间
- times()

### 守护进程
- sesion 标识是`sid`
- 终端
- setsid()
- getpgrp()
- getpgid()
- setpgid()


### 系统日志
- syslogd 服务
- openlog
- syslog
- closelog

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

#define FNAME "/tmp/out"

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
        printf("test");
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

int main()
{
    FILE* fp;

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

    closelog();
    fclose(fp);
    exit(0);
}

~~~

~~~ bash
 journalctl _PID=XXX

~~~

@[TOC](文章目录)
