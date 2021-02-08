#include <asm-generic/errno-base.h>
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static void on_signal_term(int sig){
	std::cout << "sub thread will exit" << std::endl;
	pthread_exit(NULL);
}

void *thfunc(void *arg){
	signal(SIGQUIT,on_signal_term);
	int tm = 500;
	while(true){
		std::cout << "thrfunc--left:"<<tm<<"s--"<<std::endl;
		sleep(1);
		tm --;
	}
	return (void *)0;

}
int main(int argc,char **argv){
	pthread_t pid;
	int res;

	res = pthread_create(&pid,NULL,thfunc,NULL);

	sleep(10);
	int kill_res = pthread_kill(pid,0);
	if(kill_res == ESRCH)
	  std::cout << "the specified thread did not exits or already quit\n";
	else if(kill_res == EINVAL)
	  std::cout << "signal id invalid\n";
	else
	  std::cout << "the specified thread is alived\n";
	//pthread_kill(pid,SIGQUIT);
	//pthread_join(pid,NULL);

	//std::cout << "sub thread has completed ,main thread will exit\n";
	exit(0);
}

