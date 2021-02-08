#include<stdio.h>  
#include<stdlib.h>  
#include <pthread.h>  
#include <unistd.h> //sleep

void mycleanfunc(void *arg) //清理函数
{
	printf("mycleanfunc:%d\n", *((int *)arg));						 
}
 
void *thfunc(void *arg)  
{  
	int i = 1;  
	printf("thread start-------- \n"); 
	pthread_cleanup_push(mycleanfunc, &i); //把清理函数压栈
	while (1)  
	{
		i++;  
		printf("i=%d\n", i);
	}	
	printf("this line will not run\n");
	pthread_cleanup_pop(0);
	
	return (void *)0;  
}  
int main()  
{  
	void *ret = NULL;  
	int iret = 0;  
	pthread_t tid;  
	pthread_create(&tid, NULL, thfunc, NULL);  //创建线程
	sleep(1);  
          
	pthread_cancel(tid); //发送取消线程的请求  
	pthread_join(tid, &ret);  //等待线程结束
	if (ret == PTHREAD_CANCELED) //判断是否成功取消线程
		printf("thread has stopped,and exit code: %d\n", ret);  //打印下返回值，应该是-1
	else
		printf("some error occured");
          
	return 0;  
}
