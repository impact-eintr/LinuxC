## 进程环境
### main函数
~~~c
int main(int argc,char *argv[])
~~~
### 进程的终止
#### 正常终止
- **从main函数返回**
- **调用exit**
- **调用_exit或_Exit**
	- **_exit使用时不使用钩子函数，直接返回(几乎相当于异常结束)**
- 最后一个线程从其启动例程返回
- 最后一个线程调用pthread_exit
#### 异常终止
- 调用abort
- 接到一个信号
- 最后一个线程对取消请求作出响应
### 命令行参数的分析
~~~c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * -y year
 * -m month
 * -d day
 *  -H hour
 *  -M mintue
 * 0S second
 * */
 #define TIMESTRSIZE 1024
 #define FMTSTRSIZE 1024

 int main(int argc, char** argv)
 {
 time_t stamp;
 struct tm* tm;
 char timestr[TIMESTRSIZE];
 char fmtstr[FMTSTRSIZE];
 int c = 0;

 stamp = time(NULL);
 tm = localtime(&stamp);
 fmtstr[0] = '\0';

 while (1) {
 c = getopt(argc, argv, "H:MSy:md"); //optstring is a string containing the legitimate option characters
 if (c < 0) {
 break;
 }
 switch (c) {
 case 'H':
 if (strcmp(optarg, "12") == 0) {
 strncat(fmtstr, "%I(%P) ", FMTSTRSIZE - 1);
 } else if (strcmp(optarg, "24") == 0) {
 strncat(fmtstr, "%H ", FMTSTRSIZE - 1);
 } else
 fprintf(stderr, "不合适的格式..");
 break;
 case 'M':
 strncat(fmtstr, "%M ", FMTSTRSIZE - 1);
 break;
 case 'S':
 strncat(fmtstr, "%S ", FMTSTRSIZE - 1);
 break;
 case 'y':
 if (strcmp(optarg, "2") == 0) {
 strncat(fmtstr, "%y ", FMTSTRSIZE - 1);
 } else if (strcmp(optarg, "4") == 0) {
 strncat(fmtstr, "%Y ", FMTSTRSIZE - 1);
 } else
 fprintf(stderr, "不合适的格式..");
 break;
 case 'm':
 strncat(fmtstr, "%m ", FMTSTRSIZE - 1);
 break;
 case 'd':
 strncat(fmtstr, "%d ", FMTSTRSIZE - 1);
 break;
 default:
 break;
 }
 }
 strftime(timestr, TIMESTRSIZE, fmtstr, tm);
 puts(timestr);

 exit(1);
 }

 ~~~
 ### 环境变量
 **KEY = VAL**
- getenv()
- setenv() 释放掉原来的值
- putenv()
### c程序的存储空间布局
pmap(命令)
### 库
- 动态库
- 静态库
- 手工装载库
- dlopen()
- dlclose()
- dlerror()
- dlsym
~~~c
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>  /* Defines LIBM_SO (which will be a
string such as "libm.so.6") */
int
main(void)
{
void *handle;
double (*cosine)(double);
char *error;

handle = dlopen(LIBM_SO, RTLD_LAZY);
if (!handle) {
fprintf(stderr, "%s\n", dlerror());
exit(EXIT_FAILURE);
}

dlerror();    /* Clear any existing error */

cosine = (double (*)(double)) dlsym(handle, "cos");

/* According to the ISO C standard, casting between function
pointers and 'void *', as done above, produces undefined results.
POSIX.1-2001 and POSIX.1-2008 accepted this state of affairs and
proposed the following workaround:

*(void **) (&cosine) = dlsym(handle, "cos");

This (clumsy) cast conforms with the ISO C standard and will
avoid any compiler warnings.

The 2013 Technical Corrigendum 1 to POSIX.1-2008 improved matters
by requiring that conforming implementations support casting
'void *' to a function pointer.  Nevertheless, some compilers
(e.g., gcc with the '-pedantic' option) may complain about the
cast used in this program. */

error = dlerror();
if (error != NULL) {
fprintf(stderr, "%s\n", error);
exit(EXIT_FAILURE);
}

printf("%f\n", (*cosine)(2.0));
dlclose(handle);
exit(EXIT_SUCCESS);
}


~~~
### 函数跳转
"安全的"`gpto`
- setjump()
- longjmp()
### 资源的获取及控制
