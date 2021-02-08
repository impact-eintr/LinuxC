## linux系统编程
### gcc
#### gcc编译执行程序4步骤

- 预处理 -E .i
	- 展开宏、头文件
	- 替换条件编译
	- 删除注释、空行、空白
- 编译 -S .s
	- 检查语法规范
	- 消耗时间爱呢、系统资源最多
- 汇编 -c .o
	- 将汇编指令翻译成机器指令
- 链接 没有参数 a.out(**-o 是制定输出文件名**)
	- 数据段合并
	- 数据地址回填
#### gcc重要参数
##### 指定头文件
~~~bash
./hello.c
./lib/hello.h

gcc -I ./lib hello.c -o hello
~~~
##### gdb 调试信息
~~~bash
gcc -g hello.c -o hello
~~~
##### 优化级别
`-On` **n=0~3，n越大优化越多,默认是2**
~~~bash
gcc -O0 hello.c -o hello
~~~
##### 警告级别
~~~bash
gcc hello.c -o hello -Wall #最高警告级别
~~~
##### 注册一个宏
**向程序中“动态”注册宏定义**，用于设置“开关”，设置宏就开启调试，不设置就不开启
~~~c
#include "hello.h"

#ifdef HELLO
#define HI 20
#endif

int main(){
printf("%d",HI);
return 0;
}
~~~

~~~bash
gcc -I lib -D HELLO hello.c -o hello
~~~

