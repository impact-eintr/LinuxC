### 被动端
1. 取得SOCKET
2. 给SOCKE取得地址
3. 收发信息
4. 关闭SOCKET
### 主动端
1. 取得SOCKET
2. 给SOCKET取得地址（可省略）
3. 收发信息
4. 关闭SOCKET

## 报式套接字
~~~c
socket();
bind();
sendto();
rcvfrom();
inet_pton();
inet_ntop();
setsockopt();
getsockopt();
~~~
#### 多点通讯
##### 广播
- 全网广播
- 子网广播
##### 多播/组播

