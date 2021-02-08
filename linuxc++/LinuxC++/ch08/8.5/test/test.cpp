#include <pthread.h>
#include <stdio.h> 

int gn = 10; //定义一个全局变量，将会会在主线程和子线程中用到
void *thfunc(void *arg)
{
	gn++;	//递增1
	printf("in thfunc:gn=%d,\n", gn); //打印全局变量gn值
	return (void *)0;
}

int main(int argc, char *argv [])
{
	pthread_t tidp;
	int ret;
	 
	ret = pthread_create(&tidp, NULL, thfunc, NULL);
	if (ret)
	{
		printf("pthread_create failed:%d\n", ret);
		return -1;
	}
	pthread_join(tidp, NULL); //等待子线程结束
	gn++; //子线程结束后，gn再递增1
	printf("in main:gn=%d\n", gn); //再次打印全局变量gn值
	 
	return 0;
}
