#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <unistd.h> //sleep
using namespace std;

static void on_signal_term(int sig)
{
	cout << "sub thread will exit" << endl;
	pthread_exit(NULL);
}
void *thfunc(void *arg)
{
	signal(SIGQUIT, on_signal_term);

	int tm = 50;
	while (true)
	{
		cout << "thrfunc--left:"<<tm<<" s--" <<endl;
		sleep(1);
		tm--;
	}

	return (void *)0;
}

int main(int argc, char *argv[])
{
	pthread_t     pid;
	int res;

	res = pthread_create(&pid, NULL, thfunc, NULL);

	sleep(5);

	pthread_kill(pid, SIGQUIT);
	pthread_join(pid, NULL);
	cout << "sub thread has completed,main thread will exit\n";

	return 0;
}
