#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> //strerror
 
void mycleanfunc(void *arg)
{
	printf("mycleanfunc:%d\n", *((int *)arg));						 
}
void *thfrunc1(void *arg)
{
	int m=1,n=2;
	printf("thfrunc1 comes \n");
	pthread_cleanup_push(mycleanfunc, &m); 
	pthread_cleanup_push(mycleanfunc, &n); 
	pthread_cleanup_pop(1);
	pthread_exit(0);
	pthread_cleanup_pop(0);
}
  
int main(void)
{
	pthread_t pid1 ;
	int res;
	res = pthread_create(&pid1, NULL, thfrunc1, NULL);
	if (res) 
	{
		printf("pthread_create failed: %d\n", strerror(res));
		exit(1);
	}
	pthread_join(pid1, NULL);
	
	printf("main over\n");
	return 0;
}
