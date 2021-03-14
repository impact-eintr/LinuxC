# 基本概念

## 怎么写代码
### 头文件的重要性
在c中，如果没有出现函数原型，就默认函数的返回值是int
~~~ c
#include <stdio.h>

int main()
{
    int *num = (int *)malloc(sizeof(int));
    return 0;
}
~~~

~~~ bash
hello.c: 在函数‘main’中:
hello.c:5:23: 警告：隐式声明函数‘malloc’ [-Wimplicit-function-declaration]
    5 |     int *num = (int *)malloc(sizeof(int));
          |                       ^~~~~~
          hello.c:5:23: 警告：隐式声明与内建函数‘malloc’不兼容
~~~
- 正确写法
~~~ c
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int *num = (int *)malloc(sizeof(int));
    return 0;
}
~~~

### 以函数为单位编写程序

~~~ c

~~~

## 



## 

## 

## 
