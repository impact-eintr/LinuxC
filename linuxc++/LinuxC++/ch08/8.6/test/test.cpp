#include <iostream>
#include <pthread.h>
#include <unistd.h> //sleep
using namespace std;

void *thfunc(void *arg)
{
	cout<<("sub thread is running\n");
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	size_t stack_size;
	int res;

	res = pthread_attr_init(&thread_attr);
	if (res)
		cout<<"pthread_attr_init failed:"<<res<<endl;

	res = pthread_attr_setdetachstate( &thread_attr,PTHREAD_CREATE_DETACHED);
	if (res)
		cout<<"pthread_attr_setdetachstate failed:"<<res<<endl;
 
	res = pthread_create(   &thread_id, 	&thread_attr, thfunc,
		NULL);
	if (res )
		cout<<"pthread_create failed:"<<res<<endl;
	cout<<"main thread will exit\n"<<endl;
	
	sleep(1);
  
	return 0;
}
