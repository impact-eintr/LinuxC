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

# 基本概念

## 怎么写代码
### 头文件的重要性
在c中，如果没有出现函数原型，就默认函数的返回值是int
~~~ c
#include <stdio.h>

int main()
{
<<<<<<< HEAD
    int *num = malloc(sizeof(int));
    *num = 100;
    printf("%d\n",*num);
=======
    int *num = (int *)malloc(sizeof(int));
>>>>>>> d8201497ccb6ce60411ef4f1d6347921a54e7c22
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

## 数据类型 运算符 表达式
- 基本类型
  - 数值类型
    - short
    - int
    - long
    - float
    - double
  - 字符类型
- 构造类型
  - 数组
  - 结构体 struct
  - 共用体 union
  - 枚举类型 enum
- 指针类型
- 空类型 void

``` bash
254 -> unsigned int -> 32bit
(254)10 = (1111 1110)2 = (376)8 = (FE)16

254
B11111110(c不认识这个表示)
0376
0xFE
```


#### 类型转换
``` c
int i;
float f;
doubel d;
char ch;

ch + i -> i
f -d -> d

(ch + i) - (dloat -double) -> double

```

#### bool

``` c
#incldue <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
  bool a = false;
  printf("a = %d\n", a);
  exit(0);
}
```

#### 浮点型的失真问题

``` c++
int func(float f) {
  if (f < 0) {
    return -1;
  } else if (fabs(f-0) <= 1e-6) {
    return 0;
  } else {
  return 1;
  }
}
```

####  char

**在iso c中 `char`有无符号是未定义行为**

#### 0

``` c++
0(整形) '0'(字符常量) "0"(字符串常量) '\0'(字符常量)
```

#### 输入输出
数据类型要和后续代码中所使用的输入输出要相匹配(小心自相矛盾)

``` c++
#include <stdlib.h>
#include <stdio.h>

int main() {
  unsigned int a;
  a = 1 << 31;
  printf("%d", a);
}

```

正确

``` c++
#include <stdlib.h>
#include <stdio.h>

int main() {
  unsigned int a;
  a = 1 << 31;
  printf("%ud", a);
}

```

### 常量与变量
#### 常量
- 整形常量 1 890
- 实型常量 1.2 3.14
- 字符常量 '\t' '\n' '\0' '\015'(8进制) '\x7f' '\018'(错误的表示！！)
- 字符串常量 "" "a" "abXYZ" "abc\n\021\010"(a b c \n \021 \0 1 8)
- 标识常量 
##### 宏的用法
``` c++
#include <stdlib.h>
#include <stdio.h>

#define PI 3.1415926
#define ADD 2+3
// 正确写法
//#define ADD (2+3)
int main() {
  printf("%f\n", PI);
  printf("%d\n", ADD * ADD);
}

```

``` c++
# 6 "pi.c"
int main() {
  printf("%f\n", 3.1415926);
  printf("%d\n", 2+3 * 2+3);
}

```

``` c++
#include <stdlib.h>
#include <stdio.h>

#define MAX(a,b) ({int A=a,B=b; ((A) > (B) ? (A) : (B));})

int main() {
  int a = 3, b = 5;
  printf("%d\n",MAX(a++, b++));
  printf("%d\n",MAX(a++, b++));
}

```

``` c++
#include <stdlib.h>
#include <stdio.h>

#define MAX(a,b) ({typeof(a) A=a,B=b; ((A) > (B) ? (A) : (B));})

int main() {
  int a = 3, b = 5;
  printf("%d\n",MAX(a++, b++));
  printf("%d\n",MAX(a++, b++));
}

```

在程序的预处理阶段，占编译时间，不占运行时间(没有函数调用的消耗)，但不检查语法(比较危险)


#### 变量
[存储类型] 数据类型 标识符 = 值
            TYPE     NAME  = VALUE

- auto 默认 自动分配空间

- register 建议型 寄存器类型 只能定义局部变量，不能定义全局变量，大小有限制，只能定义32位大小的数据类型，比如double就不可以。因为寄存器没有地址，所以一个register类型的变量无法打印出地址查看或使用。

- static 静态型 自动初始化为0值或空值 并且static变量的值有继承性。另外常用来修饰一个变量或者函数(防止当前函数对外扩展)

``` c++
#include <stdlib.h>
#include <stdio.h>

void func() {
  static int x = 1;
  x++;
  printf("%d\n", x);
}

int main() {
  func();
  func();
  func();
}

2
3
4
```

- extern 说明型 意味着不能改变被说明的量的值或类型 可以用来扩展外部变量的作用域

``` c++
#ifndef EXTERN_H__
#define EXTERN_H__

void func();

#endif
```

``` c++
#include "extern.h"

extern int i; // 不定义 而是引用了其他地方的i
int func() {
  printf("[%s]%d\n", __FUNCTION__, i);
}
```


``` c++
#include "stdlib.h"
#include "stdio.h"
#include "extern.h"

int i = 10;

int main() {
  printf("[%s]%d\n", __FUNCTION__, i);
}
```

### 表达式

#### 逻辑运算符的短路性
``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  int a = 1, b = 2, c = 3, d = 4;
  int m = 1, n = 1;
  
  (m = a > b) && (n = c > d);
  
  printf("m = %d\n n = %d\n", m, n); // m : 0 n : 1
}
```

#### sizeof

``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("%lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
         sizeof(int),sizeof(short), sizeof(long),
         sizeof(double), sizeof(float), sizeof(char), sizeof(void*));
}

4, 2, 8, 8, 4, 1, 8

```

#### 位运算

- | 按位或
- & 按位与
- ^ 按位异或
- ~ 按位取反

> 应用

将操作数中的第n位置1 其他位不变 num = num | 1 << n;
将操作数中的第n位置0 其他位不变 num = num & ~(1<<n);
测试第n位: if(num & (1<<n))


## IO
- printf

#### 变长参数 

``` c++
int main() {
  int i = 123;
  printf("%4d\n", i);
  
  float f = 1.23455;
  printf("%.3f\n", f);
  
  char* s= "helloworld";
  printf("%10.5s\n", s);
}
```

#### 刷新缓冲区


``` c++
int main() {
  printf("[%s:%d] before while().", __FUNCTION__, __LINE__);
  while(1);
  printf("[%s:%d] after while().", __FUNCTION__, __LINE__);
}
```

正确写法
``` c++
#include <stdlib.h>
#include <stdio.h>

int main() {
  printf("[%s:%d] before while().]\n", __FUNCTION__, __LINE__);
  // 或者
  //printf("[%s:%d] before while().", __FUNCTION__, __LINE__);
  //fflush(stdout); 
  while(1);
  printf("[%s:%d] after while().", __FUNCTION__, __LINE__);
}

```

- scanf

``` c++
int main() {
  int i;
  scanf("%d", &i);
  printf("%d\n", i);
}
```

> scanf 在使用 `%s` 的时候要特别小心

``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  char S[3];

  scanf("%s", S); // 如果输入 abcdef
  printf("%s", S); // 可能会出现段错误
}

```

> scanf 在循环中使用的时候要特别小心

``` c++
int main() {
  int ret = 0;
  int d = 0;
  
  while(1) {
    ret = scanf("%d, d);
    if (ret == -1) {
      perror("Error");
      break;
    }
    printf("&d\n", d);
  }
  exit(0);
}
```

> 处理换行

``` c++
int main() {
  int i = 0;
  char c = 0;
  
  scanf("%d", &i);
  scanf("%*c%c", &c);
  // 或者
  //getchar();
  //scanf("%c", &c);
  printf("i = %d, c = %c", i, c);
}
```


# 流程控制
跳过

# 数组
构造类型 连续存放
## 一维数组
[存储类型] 数据类型 标识符[下标]
### 初始化

- static

``` c++
static int a[10];
```

- {}

``` c++
int a[3] = {1, 2, 3};
```

### 元素引用
- arr[i]
- arr+i

#### 数组名

一个**常量**

``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  int arr[3] = {1, 2, 3};
  printf("%ld\n", sizeof(arr));
  // 下面这句是错的
  arr = {4, 5, 6};
  for (int i = 0;i < sizeof(arr)/sizeof(int);i++) {
    printf("%d", *(arr+i));
  }
}

```



### 数组越界
c对数组不进行越界检查，需要格外小心

### 练习

``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  int fib[10] = {1, 1};

  for (int i = 2;i < 10;i++) {
    fib[i] = fib[i-1]+ fib[i-2];
  }
  for (int i = 0;i < 10;i++) {
    printf("%d ", fib[i]);
  }
}

```


``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  int arr[] = {2, 3, 5, 4, 6, 7, 1, 9};
  for (int i = 0;i < sizeof(arr)/sizeof(int);i++) {
    for (int j = 0;j < sizeof(arr)/sizeof(int)-1-i;j++) {
      if(arr[j] > arr[j+1]) {
        int tmp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = tmp;
      }
    }
  }
  for (int i = 0;i < sizeof(arr)/sizeof(int);i++) {
    printf("%d ", arr[i]);
  }
}

```

``` c++
#include <stdio.h>
#include <stdlib.h>

int main() {
  int arr[] = {3, 2, 5, 4, 9, 7, 1, 6};
  for (int i = 0;i < sizeof(arr)/sizeof(int);i++) {
    int m = i;
    for (int j = i+1;j < sizeof(arr)/sizeof(int);j++) {
      if(arr[j] < arr[m]) {
        m = j;
      }
    }
    if (m != i) {
      int tmp = arr[i];
      arr[i] = arr[m];
      arr[m] = tmp;
    }
  }
  for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
    printf("%d ", arr[i]);
  }
}

```



## 二维数组
[存储类型] 数据类型 标识符[行下标][列下标]

``` c++
int main() {
  int a[M][N] = {1, 2, 3, 4, 5};
  int b[][N] = {1, 2, 3, 4, 5};
  int c[M][] = {1, 2, 3, 4, 5}; // 错误
  for (int i = 0;i < M;i++) {
    for (int j = 0;j < N;j++) {
      printf("%d ", *(a+i+j*));
    }
  }
}
```

#### 深入理解二维数组
a[2][3] = b[3] + c[3]
a[0] = b[0]
a[1] = c[0]


## 字符数组
### 定义以及初始化
[存储类型] char 标识符[]

**注意部分初始化的时候，剩余部分会自动初始化为'\0'**

### IO
scanf 无法获取带有分隔符的字符串(`\t`, `\n`, ` `)


### 常用函数
- strlen & sizeof
- strcpy & strncpy
- strcat & strncat
- strcmp & strncmp

> 单词统计

``` c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRSIZE 1024

int main() {
  char str[STRSIZE] = {};
  fgets(str, STRSIZE, stdin);
  int count= 0, flag = 0;

  for (int i = 0;str[i] != '\0';i++){
    if (str[i] == ' ') {
      flag = 0;
    } else if(!flag) {
      count++;
      flag = 1;
    }
  }
  printf("%d\n", count);
}

```

# 指针



























# 函数



























# 构造类型





