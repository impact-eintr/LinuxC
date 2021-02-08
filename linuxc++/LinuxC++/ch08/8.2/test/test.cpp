#include <pthread.h>
#include <stdio.h> 

void *thfunc(void *arg)
{
	int *pn = (int*)(arg); //获取参数的地址
	int n = *pn; 
	 
	printf("in thfunc:n=%d\n", n);
    return (void *)0; 
}
int main(int argc, char *argv [])
{
	pthread_t tidp;
	int ret, n=110;

	ret = pthread_create(&tidp, NULL, thfunc, &n);//创建线程并传递n的地址
	if (ret)
	{
		printf("pthread_create failed:%d\n", ret);
		return -1;
	}
	 
	pthread_join(tidp,NULL); //等待子线程结束
	printf("in main:thread is created\n");
	 
	return 0;
}
