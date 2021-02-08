## 进程间通信及同步
linux 提供的IPC机制
- 信号（signal），用来表示事件的发生
- 管道(亦即shell用户所熟悉的`|`操作符)，和FIFO，用于在进程间传递数据
- 套接字，供同一台主机或联网的不同主机上所运行的进程之间传递数据
- 文件锁定，为防止其他进程读取或者文件内容
- 消息队列，用于在进程间交换消息
- 信号量(semaphore)，用于同步进程动作
- 共享内存，允许多个进程共享同块内存。
~~~bash
ipcs

--------- 消息队列 -----------
键        msqid      拥有者  权限     已用字节数 消息      

------------ 共享内存段 --------------
键        shmid      拥有者  权限     字节     连接数  状态      
0x00000000 32782      yixingwei  600        58176      2          目标       
0x512e0014 19         yixingwei  600        33024      1                       
0x00000000 31         yixingwei  600        524288     2          目标       
0x00000000 57         yixingwei  600        2097152    2          目标       
0x00000000 60         yixingwei  600        3582720    2          目标       

--------- 信号量数组 -----------
键        semid      拥有者  权限     nsems     
0x512e0018 3          yixingwei  600        1         
~~~
## XSI -> SysV
- IPC -> Inter-Process Cpmmunication
- 主动端：**先发包的一端**
- 被动端：**先收包的一端（先运行）**
### Message Queues
##### megget()
##### megop()
##### msgctl()
### Semaphore(信号量) Arrays
### Shared Memory