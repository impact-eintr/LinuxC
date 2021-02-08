#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

void *thfunc(void *arg){
	int i = 1;
	printf("thread start ------\n");
	while(1)
	  i++;
	return (void *)0;
}

int main(){
	void * ret = NULL;
	int iret = 0;
	pthread_t tid;
	pthread_create(&tid,NULL,thfunc,NULL);
	sleep(1);

	pthread_cancel(tid);
	pthread_join(tid,&ret);
	if(ret == PTHREAD_CANCELED)
	  printf("OK\n");
	else
	  printf("pthread_cancel no!\n");
}
