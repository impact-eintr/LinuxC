
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fd;
	struct stat buf;
	stat("test.txt", &buf);//1
	printf("1.link =% d\n", buf.st_nlink);//未打开文件之前测试链接数

	fd = open("test.txt", O_RDONLY);//2.打开已存在文件test.txt
	stat("test.txt", &buf);
	printf("2.link =% d\n", buf.st_nlink);//测试链接数

	close(fd);//3.关闭文件test.txt
	stat("test.txt", &buf);
	printf("3.link =% d\n", buf.st_nlink);//测试链接数

	link("test.txt", "test2.txt");//4.创建硬链接test2.txt
	stat("test.txt", &buf);
	printf("4.link =% d\n", buf.st_nlink);//测试链接数

	unlink("test2.txt");//5.删除test2.txt
	stat("test.txt", &buf);
	printf("5.link =% d\n", buf.st_nlink);//测试链接数

	//6.重复步骤2  //重新打开test.txt
	fd = open("test.txt", O_RDONLY);//打开已存在文件test.txt
	stat("test.txt", &buf);
	printf("6.link =% d\n", buf.st_nlink);//测试链接数
	
	
	unlink("test.txt");//7.删除test.txt
	fstat(fd, &buf);
	printf("7.link =% d\n", buf.st_nlink);//测试链接数

	close(fd);//8.此步骤可以不显示写出，因为进程结束时，打开的文件自动被关闭。
	
}