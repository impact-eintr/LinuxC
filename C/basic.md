# 基本概念

## 头文件的重要性

~~~ c
#include <stdio.h>

int main()
{
    int *num = malloc(sizeof(int));
    *num = 100;
    printf("%d\n",*num);
    return 0;
}

~~~

~~~ bash
hello.c: 在函数‘main’中:
hello.c:5:16: 警告：隐式声明函数‘malloc’ [-Wimplicit-function-declaration]
    5 |     int *num = malloc(sizeof(int));
          |                ^~~~~~
          hello.c:5:16: 警告：隐式声明与内建函数‘malloc’不兼容
~~~

## 

## 

## 

## 
