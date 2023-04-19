I/O Input & Outout

文件分类

1. regular file 
    - text
    - binary
2. Directory
其实就是一个数组,包含 `.` `..` 其他目录 与普通文件


3. socket 插座 网络专用 针对网络中的两台主机 各自生成一个socket文件 符合条件的socket文件可以插在一起，进行通信

4. pipe 管道文件

5. 

进程中有一个FD (file descripter table) 这是一个数组 每一个项目对应着一个打开的文件(注意一切皆为文件) 这个表是进程私有的

``` sh
                             inode
| 0 | ---> [fd|ref+=1] ----> | 文件的相关属性 |
| 1 |      (kernel)          | access|size.. |
| 2 |
|   |
|   |
| n |
```


