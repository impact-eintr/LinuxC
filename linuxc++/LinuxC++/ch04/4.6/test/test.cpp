#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(void)
{
    int fd = -1,i;
    ssize_t size =-1;
    int input =0; 
    
    char buf[] = "boys and girls\n hi,children!";  
    char filename[] = "test.txt";
    
    fd = open(filename,O_RDWR|O_APPEND);  
    if(-1==fd)
    {   
        printf("Open file %s faliluer \n",filename );
    }else{
        printf("Open file %s success \n,=",filename );
    }   
    size = write(fd,buf,strlen(buf));  
    printf("write %d bytes to file %s\n",size,filename);
    
    close(fd);
    return 0;
}
