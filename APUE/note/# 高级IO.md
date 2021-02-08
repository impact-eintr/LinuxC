# 高级IO
## 非阻塞IO
##### 简单流程
自然流程是结构化的
##### 复杂流程
自然流程不是结构化的
## IO多路转接
- 监视文件描述符的状态
### 可移植
#### select()
~~~c
int select(int hfds,fd_set *readfds,fd_set *writefds,
			fd_set *exceptfds,struct timeval *timeout);
~~~
#### poll()
### 不可移植
#### epoll() --linux

## 其他读写函数
## 存储映射IO
## 文件锁