 
#include <unistd.h>  
      
int main(void)  
{  
    //执行/bin目录下的ls  
    //第一个参数为程序名ls，第二个参数为-al,第三个参数为/etc/passwd  
	execl("/bin/ls", "ls", "-al", "/etc/passwd", NULL);  
	return 0;  
}  