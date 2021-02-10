# 文件系统

<hr/>
### 目标
类似`ls`的实现

## 目录与文件
### 获取文件属性
- stat 通过文件路径获取属性
- fstat 通过文件描述符获取属性
- lstat 面对符号链接文件时，获取的是符号🔗文件`l`的属性,而`stat`获取的是链接对象的属性
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static off_t flen(const char *fname){
    struct stat statres;
    if (stat(fname,&statres) < 0) {
        perror("tata()");
        exit(1);
    }
    return statres.st_size;
}

int main(int argc,char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    long len = flen(argv[1]);
    printf("st_size = %ld\n",len);

    exit(0);
}

~~~

注意，在unix中文件大小`size`只是一个属性，不一定代表文件真正的大小(与文件系统相关)

~~~ c
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc,char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    int fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }

    long err = lseek(fd,5LL*1024LL*1024LL*1024LL-1LL,SEEK_SET);
    if (err == -1) {
        perror("lseek");
        exit(1);
    }

    write(fd,"",1);

    return 0;
}

~~~
### 文件访问权限
- _ _ _ (7种文件类型) _ t(粘住位) _ g+s _ u+s _ _ _ user _ _ _  group _ _ _ other共15位用16位的位图表示
**7种文件类型** `b c d - l s p` `不错的-老色批`
- b 块设备文件
- c 字符设备文件
- d 目录
- - 常规文件
- l 符号链接文件
- s socket文件
- p 匿名管道文件(不可见)
~~~ c

//文件类型
static int ftype(const char* fname) {
    if (stat(fname,&statres) < 0) {
        perror("rstat()");
        exit(1);
    }
    if (S_ISREG(statres.st_mode)) {
        return '-';
    }else if (S_ISDIR(statres.st_mode)) {
        return 'd';
    }else{
        return '?';
    }
~~~
### umask
- 防止产生权限过松的文件
### 文件权限的更改与管理
- chmod (命令)
    - chmod a+x ??x ??x ??x
    - chmod u+x ??x ??? ???
    - chmod g+x ??? ??x ???
    - chmod o+x ??? ??? ??x
- chmod()

~~~ c
#include <sys/stat.h>
int chmod(const char *path,mode_t mode);
int fchmod(int fd,mode_t mode); //修改一个已经成功打开的文件

~~~
### 粘住位
- `t位`
~~~ bash
$ ls -l /
drwxrwxrwt   1 root root 3.6K 2月   8 17:58 tmp
~~~
### 文件系统的实质
**FAT UFS**
文件系统： 文件或数据的存储和管理

### 硬链接 符号连接
- link (命令) 创建`硬链接` 其实就是在`目录项`中添加一条映射
- ln() => ln
- unlink() 删除一个文件的硬连接 但并不是删除文件 只有当一个文件的硬链接数为0 且没有进程占用该文件时一个文件才有被删除的可能（数据可被随意改写）

### 文件读写时间
- utime()

### 目录的创建和销毁
- mkdir ()
- rmdir()

### 更改当前工作路径
- chdir => cd

### 分析目录/读取目录内容
单独调用
- glob **解析模式/通配符**

~~~ c
//glob解析路径
static void Glob(){
    glob_t globres;
    int err = glob(PAT,0,&errfunc,&globres);
    if (err) {
        printf("Error code = %d\n",err);
    }

    for (int i = 0;globres.gl_pathv[i]!= NULL;i++) {
        fprintf(stdout,"%s\n",globres.gl_pathv[i]);
    }
}

~~~

组合调用
- opendir()
- closedir()
- readdir()
- seekdir()
- telldir()

~~~ c
//组合解析路径
static void PathParse(char *Path) {
    DIR *dp;
    struct dirent *cur;

    dp = opendir(Path);
    if (dp == NULL) {
          perror("opendir");
          exit(1);
    }

    while((cur = readdir(dp)) != NULL) {
        fprintf(stdout,"%s ",cur->d_name);
        fprintf(stdout,"type:%d ",cur->d_type);
    }

    closedir(dp);
}

    //getcwd()的使用
    char pwd[1024];
    getcwd(pwd,1024);
    fprintf(stdout,"%s\n",pwd);
    PathParse(pwd);
~~~

- 综合案例 `mydu.c`

~~~ c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>
#include <string.h>

#define PATHSIZE 1024

static int path_noloop(const char *path) {
    char *pos;
    pos = strrchr(path,'/');
    if (pos == NULL) {
        exit(1);
    }

    if (strcmp(pos+1,".") == 0||strcmp(pos+1,"..")== 0) {
        return 0;
    }
    return 1;
}

static int64_t mydu(const char *path) {
    static struct stat statres;
    static char nextpath[PATHSIZE];
    glob_t globres;
    int64_t sum = 0;

    //非目录
    if (lstat(path,&statres) < 0) {
        perror("lstat()");
        exit(1);
    }

    if (!S_ISDIR(statres.st_mode)){
        fprintf(stdout,"%ld\t%s\n",statres.st_blocks / 2,path);
        return statres.st_blocks;
    }
    //目录
    //拼接路径
    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/*",PATHSIZE);
    if (glob(nextpath,0,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/.*",PATHSIZE);
    if (glob(nextpath,GLOB_APPEND,NULL,&globres) < 0) {
        fprintf(stderr,"glob()");
        exit(1);
    }

    sum = statres.st_blocks;
    for (int i = 0;i < globres.gl_pathc;i++){
        if (path_noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);
    }
    
    globfree(&globres);//回收资源
    return sum;
        
}

int main(int argc,char **argv)
{   
    if (argc < 2) {
        fprintf(stderr,"%s\n","Usage...");
        exit(1);
    }
    
    printf("%ld\t%s\n",mydu(argv[1])/2,argv[1]);

    return 0;
}

~~~
## 系统数据文件和信息
1. `/etc/passwd`
    - getpwuid();
    - getpwnam();
2. `/etc/group`
    - getgrgid();
    - getgrgrnam();
3. `/etc/shadow`
    - getspnam();
    - crypt();
    - getpass();

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>

int main(int argc,char **argv)
{
    char *input_passwd;//来自命令行的密码原文
    char *crypted_passwd;
    struct spwd *shadowline;
    
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    input_passwd = getpass("PassWoed:");

    shadowline = getspnam(argv[1]);

    crypted_passwd = crypt(input_passwd,shadowline->sp_pwdp);
    
    if (strcmp(shadowline->sp_pwdp,crypted_passwd) == 0)
      puts("ok!");
    else
      puts("failed!");

    return 0;
}

以上代码编译后(编译要加`-lcryp`) ，要使用root用户执行(普通用户没有权限)

~~~

### 时间戳
**time_t => char * => struct_tm**

- time() 从kernel中取出时间戳(以秒为单位)
- gntime() 将时间戳转换为`struct_tm` 格林威治时间
- localtime() 将时间戳转换为`struct_tm` 本地时间
- mktime() jaing struct_tm结构体转换为时间戳，还可以检查是否溢出
- strftime(); 格式化时间字符串

~~~ c
time_t stamp;
time(&stamp);
stamp = time(NULL);

tm = localtime(&stamp);

strftime(buf,BUFSIZE,"%Y-%m-%d",tm);
puts(buf);
~~~

~~~ c
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

~~~

## 进程环境
### main函数
- `int main(int argc,char **argv)`
### 进程的终止
- **正常终止**
     - **从main函数返回** 
     - **调用exit**
     - **调用`_exit`或者`_Exit`**
     - **最后一个线程从其启动例程返回**
     - **最后一个线程调用`pthread_exit`**
#### 钩子函数a
    All functions registered with atexit(3) and on_exit(3) are called,in the reverse order of their registration.
- `atexit()`
~~~ c

~~~

- **异常终止**
    - **调用`abort`**
    - **接到一个信号并终止**
    - **最后一个线程对其取消请求作出响应**

~~~ c

~~~

### 命令行参数的分析

~~~ c

//解析命令行
while(1) {
    c = getopt(argc,argv,"lt-a"); // - 识别非选项的传参
    if (c < 0){
        break;
    }
    
    switch (c){
        case 'l':
            f.filesize = flen(argv[1]);
            strncat(fmtstr,"filesize:%ld ",FMTSTRSIZE-1);
            break;
        case 't':
            f.filetype = ftype(argv[1]);
            strncat(fmtstr,"filetype:%c ",FMTSTRSIZE-1);
            break;
        case 'a':
            PathParse(argv[optind]);
            break;
        default:
            break;
    }
}
~~~

### 环境变量
**本质就是 KEY = VALUE**
- `export`
- getenv()
- setenv()
~~~ c
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

static void getEnv(char *key){
    puts(getenv(key));
}

int main()
{
    
    for (int i = 0;environ[i] != NULL;i++){
        puts(environ[i]);
    }
    getEnv("ZSH");
    return 0;
}
~~~

### C程序的存储空间布局

- pmap (1)
### 库
##### 动态库

##### 静态库

#### 手工装载库
### 函数跳转

### 资源的获取与控制
