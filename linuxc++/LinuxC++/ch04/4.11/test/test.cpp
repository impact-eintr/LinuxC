#include <sys/mman.h> /* for mmap and munmap */  
#include <sys/types.h> /* for open */  
#include <sys/stat.h> /* for open */  
#include <fcntl.h>     /* for open */  
#include <unistd.h>    /* for lseek and write */  
#include <stdio.h>  
  
int main(int argc, char **argv)  
{  
	int fd;  
	char *mapped_mem, * p;  
	int flength = 1024;  
	void * start_addr = 0;  
  
	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  
	flength = lseek(fd, 1, SEEK_END);  
	write(fd, "\0", 1); /* 在文件最后添加一个空字符，以便下面printf正常工作 */  
	lseek(fd, 0, SEEK_SET);  
	mapped_mem =(char*) mmap(start_addr,
		flength,
		PROT_READ,        //允许读  
		MAP_PRIVATE,       //不允许其它进程访问此内存区域  
		fd,
		0);  
      
/* 使用映射区域. */  
	printf("%s\n", mapped_mem); /* 为了保证这里工作正常，参数传递的文件名最好是一个文本文件 */  
	close(fd);  
	munmap(mapped_mem, flength);  
	return 0;  
}  