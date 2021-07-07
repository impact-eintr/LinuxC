# I/O
一切实现的基础
## stdio
### 打开关闭
- fopen()
    - open(linux)
    - openfile(win)
- fclose()
~~~ c
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef false
#include <stdio.h>
FILE *fopen(const char *path,const char *mode);
FILE *fdopen(int fd,const char *mode);
FILE *freopen(const char *pathmconst char *mode,FILE *)
#endif

int main()
{
#ifdef false
    char *ptr = "abc";
    ptr[0] = 'x';如果要修改的话，会报段错误。意图修改一个常量
    printf("%s\n",ptr);
#endif

    FILE *fp;
    fp = fopen("log","r");
    if (fp == NULL){
        fprintf(stderr,"fopen() faild! errno = %d\n",errno);//errno已经被私有化
        perror("fopen");
        printf("%s\n",strerror(errno));
        exit(1);
    }else{
        fputs("ok!\n",stdout);
        fputs("OK\n",fp);
    }
    return 0;
}

~~~
**如果一个系统函数有打开和其对应的关闭操作，那么函数的返回值是放在 堆 上的（malloc 与free），否则有可能在堆上也有可能在静态区**

~~~ bash
/usr/include/asm-generic/errno-base.h
/usr/include/asm-generic/errno.h
~~~

~~~c
#include <errno.h>
perror("fopen()",errno);
#include <string.h>
strerror(errno);
~~~
- 默认最多打开的文件描述符个数
~~~bash
$ ulimit -a

-n: file descriptors                1024
~~~
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    int count = 0;
    FILE *fp = NULL;

    while(1)
    {
        fp = fopen("tmp","r");
        if (fp == NULL)
        {
            perror("fopen()");
            break;
        }
        count++;
    }
    printf("%d\n",count);
    return 0;
}

~~~
#### 新文件权限产生
**mod = 0666 & ~umask**
修改方式 
~~~ bash
umask 022
~~~
### 读写
- fgetc()
- fputc()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*******
 *这节主要讲fgetc 可以用来统计文件的大小
 *
 * *****/

int main(int argc,char **argv)
{
    FILE *fp = NULL;
    int count = 0;

    if (argc < 2){
        fprintf(stderr,"Usage:\n");
        exit(1);
    }

    fp = fopen(argv[1],"r");
    if (fp == NULL){
        perror("fopen()");
        exit(1);
    }

    while(fgetc(fp) != EOF){
        count++;
    }
    printf("%d\n",count);
    fclose(fp);
    return 0;
}

~~~
- fgets()
- fputs()
cp 的fgets/fputs版本
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 1024

int main()
{
    FILE *fps,*fpd;
    fps = fopen("./tmp","r");
    if (fps == NULL){
        strerror(errno);
        exit(1);
    }

    fpd = fopen("./copy","w");
    if (fpd == NULL){
        fclose(fps);
        strerror(errno);
        exit(1);
    }

    char buf[SIZE];
    while(fgets(buf,SIZE,fps)){
        fputs(buf,fpd);
    }
    fclose(fps);
    fclose(fpd);
    return 0;
}
~~~
**fgets遇到 size-1 or '\n'停止**
~~~ base
abcd
1-> a b c d '\0'
2-> '\n' '0'
~~~
~~~ c
#define SIZE 5
int main()
{
    FILE *fp = NULL;
    fp = fopen("./tmp","r");
    if (fp == NULL){
        perror("fopen()");
        exit(1);
    }

    char buf[SIZE];

    while(fgets(buf,SIZE,fp)){
        printf("%s\n",buf);
    }
    return 0;
}
~~~
- fread()
- fwrite()
cp的fread/fwrite版本
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 1024
/***************
 *fread 实现文件复制
 *
 **************/
int main()
{
    FILE *fps,*fpd;
    fps = fopen("./tmp","r");
    if (fps == NULL){
        strerror(errno);
        exit(1);
    }

    fpd = fopen("./copy","w");
    if (fpd == NULL){
        fclose(fps);
        strerror(errno);
        exit(1);
    }

    char buf[SIZE];

    int n = 0;
    while ((n = fread(buf,1,SIZE,fps)) > 0){
        printf("%d\n",n);
        fwrite(buf,1,n,fpd);
    }
    fclose(fps);
    fclose(fpd);
    return 0;
}
~~~
### 打印与输入
- printf()
- scanf()
~~~ c
#include <stdio.h>
/*****************
 *sprintf atoi fprintf
 *sprintf 可以看作是 atoi的反向操作
 * **************/
int main()
{
    char buf[1024];
    int y = 2020;
    int m = 12;
    int d = 24;
    sprintf(buf,"%d/%d/%d",y,m,d);
    printf("%s",buf);
    return 0;
}
~~~
### 文件指针
#### 何谓“文件指针”？
就像读书时眼睛移动一样，文件指针逐行移动
#### 什么时候用？
对一个文件先读后写的时候，比如：
~~~ c
FILE *fp = fopen();
fputc(fp);

fgetc();//无法得到刚刚写入的东西
~~~

~~~ c

int fseek(FILE *stream,long offset,int whence);
//设置文件指针位置 offset是偏移位置
long ftell(FILE *stream);
//返回文件指针位置
void rewind(FILE *stream);
//使得文件指针回到文件开始位置
~~~
- fseek()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    const int SIZE = 1024;
    FILE *fp = NULL;
    char buf[SIZE];
    buf[0] = 'A';
    buf[1] = 'A';
    buf[2] = 'A';
    buf[3] = 'A';

    fp = fopen("./tmp","w+");
    if (fp == NULL){
        strerror(errno);
        exit(1);
    }
    
    int i = 0;
    while(i < 10){
        unsigned long n = fwrite(buf,1,4,fp);
        fseek(fp,-n,SEEK_CUR);
        unsigned long len =  fread(buf,1,n,fp);
        printf("%lu\n",len);
        fseek(fp,0,SEEK_END);
        i++;
    }
    
    fseek(fp,1024,SEEK_CUR);
    return 0;
}
~~~
- ftell()
- rewind()
### 刷新缓存
- fflush()
~~~ c
printf("Before while");
fflush();
while(1)
printf("After while";)
~~~
#### 缓冲区
##### 缓冲区的作用a
大多数情况是好事，合并系统调用
##### 行缓冲
换行时刷新 stdout
##### 全缓冲
默认
##### 无缓冲
stderr
### 取到完整的一行
- getline()
~~~ c
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *fp = NULL;
    fp = fopen("./tmp","r");
    if (fp == NULL){
        strerror(errno);
        exit(1);
    }

    size_t linesize = 0;
    char *line = NULL;

    while(1){
        if (getline(&line,&linesize,fp) < 0){
            break;
        }
        printf("%ld\n",strlen(line));
        printf("%ld\n",linesize);
    }
    return 0;
}
~~~
### 临时文件
- tmpfile()
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *tmpfp;
    tmpfp = tmpfile();

    const char* oribuf = "2020/12/25";
    int SIZE = 0;
    while(*(oribuf+SIZE) != 0){
        SIZE++;
    }
    printf("%d\n",SIZE);
    
    fwrite(oribuf,1,SIZE,tmpfp);
    fseek(tmpfp,0,SEEK_SET);
    

    FILE *fp;
    fp = fopen("tmp","w");
    char desbuf[SIZE];
    int n = fread(desbuf,1,SIZE,tmpfp);
    fwrite(desbuf,1,n,fp);

    fclose(tmpfp);
    fclose(fp);
    return 0;
}
~~~
## sysio(系统IO)
**fd**是文件IO中贯穿始终的类型
### 文件描述符的概念
FILE *

整型数，数组下标，文件描述符优先使用当前可用范围内最小的
### 文件IO操作
- open
- close
- read
- write
- lseek
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc,char** argv)
{
    if (argc < 3){
        fprintf(stdout,"Usage...");
        exit(1);
    }

    int sfd = open(argv[1],O_RDONLY);
    if (sfd < 0){
        strerror(errno);
        exit(1);
    }

    int dfd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(dfd < 0){
        close(sfd);
        strerror(errno);
        exit(1);
    }

    const int SIZE = 1024;
    char buf[SIZE];

    while(1){
        len = read(sfd,buf,SIZE);
        if (len < 0){
            strerror(errno);
            break;
        }
        if (len == 0){
            break;
        }
        //以防写入不足
        while(len > 0){
            int len = 0;//read的返回值
            int ret = 0;//write的返回值
            int pos = 0;//写截至的位置
            ret = write(dfd,buf+pos,len);
            if (ret < 0){
                strerror(errno);
                exit(1);
            }
            pos += ret;
            len -= ret;
        }
    }

    close(dfd);
    close(sfd);

    exit(0);
}

~~~
### 文件IO与标准IO的区别
- 区别 标准IO的吞吐量大 系统IO的响应速度快(是对程序而言，对用户而言stdio的速度“更快”)
- 转换 `fileno` `fdopen`
- 注意 标准IO与系统IO不可以混用

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    for (int i = 0;i < 1025;i++){
        putchar('a');
        write(1,"b",1);
    }

    return 0;
}

~~~

`strace`可以用来查看一个可执行文件的系统调用，使用`strace ./a.out`可以看到先进行1024次系统调用然后缓冲区满了1024合并为一次系统调用
### IO的效率问题


### 文件共享
`truncate` 或者 `ftruncate`

### 原子操作
不可分割的操作
- 作用： 解决竞争和冲突 比如`tmpnam`就操作不原子

### 程序中的重定向
- dup
将传入的文件描述符复制到可使用的(未使用的)最小新文件描述符,在下面的例子中，将标准输出关闭后，文件描述符1空闲(不发生竞争时)，`dup`将会把打开了文件`/tmp/out`的文件描述符复制给文件描述符1 ，之后对文件描述符1 的操作就相当与操作文件`/tmp/out`

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FNAME "/tmp/out"


int main()
{
    int fd;
    fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }
    //dup 不原子
    close(1);//关闭标准输出
    dup(fd);
    close(fd);
    
    /*********************/
    printf("Hello world\n");
    return 0;
}

~~~

- dup2
`dup2`可以避免关闭文件描述符后被其他线程抢占
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FNAME "/tmp/out"


int main()
{
    int fd;
    fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if (fd < 0) {
        perror("open()");
        exit(1);
    }
    //dup2 原子
    dup2(fd,1);
	
	if (fd != 1) {//打开的文件描述符如果不是1自己，就可以把他关掉了，有重定向后的 1 可以访问文件，保持尽量少的资源使用
		close(fd);
	}

    /*********************/
    printf("Hello world\n");
    return 0;
}

~~~

### 同步
- sync
设备即将解除挂载时进行全局催促，将buffer cache的数据刷新
- fsync
刷新文件的数据
- fdatasync
刷新文件的数据部分，不修改文件元数据
