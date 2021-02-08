#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void mycleanfunc (void *arg){
	printf("mycleanfunc:%d\n",*((int *)arg));
}

void *thfunc1(void *arg){
	int m = 1;
	printf("thfunc1 comes \n");
	pthread_cleanup_push(mycleanfunc,&m);
	return (void *)0;
	pthread_cleanup_pop(0);

}

void *thfunc2(void *arg){
	int m = 2;
	printf("thfunc2 comes \n");
	pthread_cleanup_push(mycleanfunc,&m);
	return (void *)0;
	pthread_cleanup_pop(0);
}



int main(){
	pthread_t pid1,pid2;
	int res;

	alarm(1);
	res = pthread_create(&pid1,NULL,thfunc1,NULL);
	if(res){
		printf("pthread_create failed:%s\n",strerror(res));
		exit(1);
	}
	pthread_join(pid1,NULL);
	res = pthread_create(&pid2,NULL,thfunc2,NULL);
	if(res){
		printf("pthread_create failed:%s\n",strerror(res));
		exit(1);
	}
	pthread_join(pid2,NULL);

	std::cout << "main thread exiting!"<<std::endl;
	pause();
	std::cout << "main thread exited!"<<std::endl;

}
