
# 概览
## 项目地址
[https://github.com/impact-eintr/LinuxC](https://github.com/impact-eintr/LinuxC)

## c的历史
- 1960 原型A语言->ALGOL语言
- 1963 CPL语言
- 1967 BCPL
- 1970 B语言
- 1973 C语言

## C语言特点
1. 基础性语言
2. 语法简洁 紧凑 方便 灵活(得益于指针)
3. 运算符 数据结构丰富
4. 结构化 模块化编程
5. 移植性好 执行效率高
6. **允许直接对硬件操作**

## 学习建议
1. 概念的正确性
2. 动手能力
3. 主动阅读优秀的程序段
4. 大量练习,编程是技术不是理论

## 学习思路
1. 基本概念
2. 数据类型 运算符 表达式
3. 输入输出
4. 流程控制
5. 数组
6. 指针
7. 函数
8. 构造类型
9. 动态内存管理
10. 常用库函数 
11. 调试工具和调试技巧


## 环境搭建与"Hello world"
## 环境
- 当前测试环境是安装了基于`archlinux`的`manjarolinux`发行版的物理机，大家自己搭建linux环境的话推荐试用或租用云服务器或者尝试`WSL`
- `gcc`版本是 10.2.0
- 编辑器使用`vim`(推荐vim配置[vimplus](https://github.com/chxuan/vimplus))
### "Hello world"
~~~ c
#inlcude <stdio.h>
#include <stdlib.h>

int main(void){
    printf("hello world\n");
    exit(0);
}
~~~

gcc 编译c的源文件过程

~~~ bash
gcc -v
~~~

**C源文件->预处理->编译->汇编->链接->可执行文件**

完整过程
- 预处理
~~~ bash
gcc -E hello.c > hello.i
~~~
- 编译
~~~ bash
gcc -S hello.i 
~~~
- 汇编
~~~ bash
gcc -c hello.s 
~~~
- 链接->可执行文件
~~~ c
gcc hello.o -o hello
~~~

或者
~~~ bash
gcc hello.c -o hello
~~~

又或者
~~~ bash
make hello
~~~

执行
~~~ bash
./hello

hello world
~~~
