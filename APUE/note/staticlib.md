## 目标库
- 将一组函数代码加以编译，并置于一个文件中，供其他应用程序调用
### 静态库
- 静态库是对已编译的目标模块的结构化整合

### 静态库与动态库的对比
- 对空间要求较低，而对时间要求较高的核心程序中
- 对时间要求较低，对空间要求较高（多数用户程序）
### 静态库的制作
- 将.c生成 .o文件
	- gcc -c file.c -o file.o
- 使用 ar 文件制作静态库
	- ar rcs lib库名.a -o file.o
- 编译静态库到可执行文件中
	- gcc test.c lib库名.a -o a.out
### 头文件守卫：防止头文件被重复包含
~~~c
#ifndef _HEAD_H_
#define _HEAD_H_

...

#endif
~~~
### 静态库的目录结构
~~~bash
➜  staticlib tree .
.
├── inc
│   └── libclc.h
├── lib
│   └── libclc.a
├── src
│   ├── add.c
│   ├── div.c
│   └── sub.c
├── test
└── test.c

3 directories, 7 files
~~~
