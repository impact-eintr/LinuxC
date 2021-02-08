#ifndef _GNU_SOURCE  
#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */  
#endif  
#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <limits.h>
static void * thread_start(void *arg)  
{  
	int i,res;  
	size_t stack_size;
	pthread_attr_t gattr;  
 
	res = pthread_getattr_np(pthread_self(), &gattr);  
	if (res)  
		printf("pthread_getattr_np failed\n");  
    
	res = pthread_attr_getstacksize(&gattr, &stack_size);
	if (res)
		printf("pthread_getattr_np failed\n"); 
	
	printf("Default stack size is %u byte; minimum is %u byte\n", stack_size, PTHREAD_STACK_MIN);
	 
	
	 pthread_attr_destroy(&gattr);  
}  
      
int main(int argc, char *argv[])  
{  
	pthread_t thread_id;  
	int s;  
 
	s = pthread_create(&thread_id, NULL, &thread_start, NULL);  
	if (s != 0)  
	{
		printf("pthread_create failed\n"); 
		return 0;
	}
	pthread_join(thread_id, NULL); //等待子线程结束
}  