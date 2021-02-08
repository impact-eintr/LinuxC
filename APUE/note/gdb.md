## GDB调试命令
### 基础指令
- -g 使用g编译可执行文件，得到调试表
- l
- b
- r
- n 下一条指令（**会越过函数**）
- s 下一条指令（**会进入函数**）
- p 查看变量的值
- c
- q
### 其他常用指令
- r 使用run 查找段错误出现位置
- finish 结束当前函数调用
- set args 设置main函数命令行参数
- run string1 string2
- info b 查看断点信息表
- `b 20 if i = 5` 设置条件断点
- ptype 查看变量类型
- bt 列出当前程序正存活着的栈帧
- frame 根据栈帧切换栈帧
- display 设置跟踪变量
- undisplay 取消
