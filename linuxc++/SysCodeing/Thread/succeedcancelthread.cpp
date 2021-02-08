#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

static int i = 0;

void *thfunc(void *arg){
	printf("thread start ------\n");
	while(1){
		i++;
		pthread_testcancel();
	}

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
	  printf("pthread_testcancel yes!\n");
	else
	  printf("pthread_cancel no!\n");

	printf("i = %d\n",i);
	exit(0);
}
