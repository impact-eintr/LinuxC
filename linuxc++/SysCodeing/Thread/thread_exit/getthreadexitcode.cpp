#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PTHREAD_NUM 2
void *threadfunc1(void *arg){
	static int count = 1;
	pthread_exit((void *)(&count));
}

void *threadfunc2(void *argv){
	static int count  = 2;
	return (void *)(&count);
}

int main(){
	pthread_t pid[PTHREAD_NUM];
	int retPid;
	int *pRet1;
	int *pRet2;

	if((retPid = pthread_create(&pid[0],NULL,threadfunc1,NULL)) != 0){
		perror("pthread_create(1)");
		exit(1);
	}

	if((retPid = pthread_create(&pid[1],NULL,threadfunc2,NULL)) != 0){
		perror("pthread_create(2)");
		exit(1);
	}
	if(pid[0] != 0){
		pthread_join(pid[0],(void **)(&pRet1));
		printf("get code1:%d\n",*pRet1);
	}

	if(pid[1] != 0){
		pthread_join(pid[1],(void **)(&pRet2));
		printf("get code2:%d\n",*pRet2);
	}
	exit(0);
}
