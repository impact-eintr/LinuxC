#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(void)
{
    int fd = -1,i;
    ssize_t size =-1;
    char buf[10];   
    char filename[] = "/root/test.txt"; //要读取的文件

    fd = open(filename,O_RDONLY); //只读方式打开文件
    if(-1==fd)
    {   
            printf("Open file %s failuer,fd:%d\n",filename,fd);
		       return -1;
    }
    else  printf("Open file %s success,fd:%d\n",filename,fd);
  
    //循环读取数据，直到文件末尾或者出错
    while(size)
    {   
       //读取文件中的数据，10的意思是希望读10个字节，但真正读到的字节数是函数返回值
        size = read(fd,buf,10);       
        if(-1==size)
        {   
            close(fd);
            printf("Read file %s error occurs\n",filename);
            return -1; 
        }else{
            if(size>0)
            {   
                printf("read %d bytes:",size);
                printf("\"");
                for(i =0;i<size;i++) //循环打印文件读到的内容
                    printf("%c",*(buf+i));
                printf("\"\n");
            }else{
                printf("reach the end of file \n");
            }

        }
    }
    return 0;
}
