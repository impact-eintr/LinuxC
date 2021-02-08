## 目录和文件
#### 获取文件属性
#### 文件访问权限
#### umask
#### 文件权限的更改/管理
#### 粘住位
#### 文件系统 FAT UFS
#### 硬链接 符号链接
- link - ln
- unlink
- remove - rm
- rename - mv
- 硬链接与目录项是同义词。且建立硬链接有限制;
	- 不能给分区建立，不能给目录建立
	- 符号链接优点：可以跨分区 可以给目录建立

#### utime
#### 目录操作
##### opendir
~~~c
DIR *opendir(char* name);
~~~
##### closedir
~~~c
int closedir(DIR * dp);
~~~
##### readdir
~~~c
strust dirent * readdir(DIR * dp);
struct dirent{
inode,
char chmane[256]
}
~~~
#### 更改当前工作路径
#### 分析目录/读取目录内容
~~~c
/*
 * 当一个源程序由多个源文件组成时，非静态的全局变量在各个源文件中都是有效的。 而静态全局变量则限制了其作用域， 即只在定义该变量的源文件内有效， 在同一源程序的其它源文件中不能使用它。由于静态全局变量的作用域局限于一个源文件内，只能为该源文件内的函数公用，因此可以避免在其它源文件中引起错误
 */
 static int errfunc_(const char* errpath, int eerrno)
 {
 puts(errpath);
 fprintf(stderr, "ERROR NESSAGE:%s\n", strerror(eerrno));
 return 0;
 }

 static int path_noloop(const char* path)
 {
 char* pos;
 pos = strrchr(path, '/'); //返回最后一个出现‘/’的字符指针
 if (pos == NULL)
 exit(1);

 if (strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0) {
 return 0;
 }
 return 1;
 }

 void println(const char* nextpath, int size)
 {

 for (int i = 0; i < size; i++) {
 printf("%c", nextpath[i]);
 }
 printf("\n");
 }

 static int64_t mydu(const char* path)
 {
 struct stat statres;
 int sum = 0;

 if (lstat(path, &statres) < 0) {
 perror("lstat()");
 exit(1);
 }
 if (!S_ISDIR(statres.st_mode)) {
 return statres.st_blocks;
 }

 char nextpath[PATHSIZE];
 glob_t globres;
 int err, size = 0;

 strncpy(nextpath, path, PATHSIZE);
 strncat(nextpath, "/*", PATHSIZE - 1);
 size = sizeof(nextpath) / sizeof(nextpath[0]);
 println(nextpath, size);
 err = glob(nextpath, 0, &errfunc_, &globres);
 if (err) {
 printf("ERROR num = %d\n", err);
 exit(1);
 }

 strncpy(nextpath, path, PATHSIZE);
 strncat(nextpath, "/.*", PATHSIZE - 1);
 //size = sizeof(nextpath) / sizeof(nextpath[0]);
 println(nextpath, size + 1);
 err = glob(nextpath, GLOB_APPEND, &errfunc_, &globres);
 if (err) {
 printf("ERROR num = %d\n", err);
 exit(1);
 }

 sum += statres.st_blocks;

 for (int i = 0; i < globres.gl_pathc; i++) {
 if (path_noloop(globres.gl_pathv[i])) {
 sum += mydu(globres.gl_pathv[i]);
 }
 }

 globfree(&globres);
 return sum;
 }

 ~~~
##### 递归遍历目录
1. 判断命令行参数
2. 判断用户指定的是否是目录
3. 读目录
~~~c
int main(int argc, char* argv[])
{
//判断命令行参数
if (argc == 1) {
isfile(".");
} else {
isfile(argv[1]);
}
return 0;
}

void isfile(char* name)
{
int ret = 0;
struct stat sb;

//获取文件属性 判断文件类型
ret = stat(name, &sb);
if (ret == -1) {
perror("stat error");
return;
}
//目录文件
if (S_ISDIR(sb.st_mode)) {
read_dir(name);
}
//普通文件
printf("%25s\t\t%ld\n", name, sb.st_size);

return;
}

void read_dir(char* dir)
{
char path[PATH_LEN];
DIR* dp;
struct dirent* sdp;

dp = opendir(dir);
if (dp == NULL) {
perror("opendir error");
return;
}
while ((sdp = readdir(dp)) != NULL) {
if (strcmp(sdp->d_name, ".") == 0 || strcmp(sdp->d_name, "..") == 0)
continue;

//拼接目录路径
sprintf(path, "%s/%s", dir, sdp->d_name);
//判断文件类型 目录递归进入 文件显示 名字 大小
isfile(path);
}
closedir(dp);
return;
}

~~~
## 系统数据文件和信息
- /etc/passwd
	- getpwuid()
	- getpwnam()
- /etc/group
	- getgrgid()
	- getgrgrnam()
- /etc/shadow
	- getspnam()
	- crypt()
### 时间戳
- time()
- gmtime()
- localtime()
- mktime()

