# 跨主机IPC(进程间通信)

## 网络套接字(socket)

## 跨主机的传输要注意的问题
### 字节序
- 大端 **低地址放高字节**
- 小端 **高地址放低字节（x86）**
~~~ c
0x00 00 00 05

//大端
05 00 00 00
//小端
00 00 00 05
~~~
- 主机字节序 **host**
- 网络字节序 **network**
- _ to _ 长度()
    - htons()
    - htonl()
    - ntohs()
    - ntohl()
### 对齐
~~~ c
struct{
    int i;
    char ch;
    float f;
};//12个字节
~~~
结构体内存对齐
- 解决方案 **指定宏 告诉编译器不对齐**
### 类型长度
- int的长度未定义
- char有无符号未定义
解决： `int32_t` `uint32_t` `int64_t` `int8_t(有符号的char)` `uint8_t(无符号的char)`

### socket
**一个中间层，连接网络协议与文件操作**

~~~ c
int socket(int domain,int type,int protocol)
          //SOCK_STREAM 有序 可靠 双工 基于(单字节)字节流
          //SOCK_DGRAM 分组的 不可靠的 无连接
          //SOCK_SEQPACKET (有序分组式) 有序可靠 报式传输
~~~
## UDP
### 步骤
- 被动端
    - 取得SOCKET
    - 给SOCKET取得地址
    - 收/发消息
    - 关闭SOCKET
- 主动端
    - 取得SOCKET
    - 给SOCKET取得地址(可以省略 本地通信地址由操作系统分配)
    - 发/收消息
    - 关闭SOCKET

~~~ c

~~~

~~~ c

~~~

~~~ c

~~~
## TCP

