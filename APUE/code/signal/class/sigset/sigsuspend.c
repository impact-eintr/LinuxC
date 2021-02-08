#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void int_handler(int s)
{
    write(1, "?", 1);
}
#if 0

#endif
int main()
{
    //mask一开始全1
    sigset_t set, o_set, save_set;
    signal(SIGINT, int_handler);
    sigemptyset(&set);	     //添加一个新的空信号集
    sigaddset(&set, SIGINT); //将SIGINT信号加入信号集 000010000000....

    sigprocmask(SIG_UNBLOCK, &set, &save_set); //mask = 11110111111... & old_mask
					       //save_set = old_mask
    sigprocmask(SIG_BLOCK, &set, &o_set);      //mask = 000010000...| old_mask
					       //o_set = old_mask

    for (int i = 0; i < 1000; i++) {
	for (int j = 0; j < 5; j++) {
	    write(1, "*", 1);
	    sleep(1);
	}
	write(1, "\n", 1);
	sigsuspend(&o_set); //等待阻塞信号集
#if 0
	sigset_t tmpset;
	sigprocmask(SIG_SETMASK, &o_set, &tmpset);//tmpset = o_set
	//SIG_SETMASK The set of blocked signals is set to the argument set.
	pause();
	sigprocmask(SIG_SETMASK, &tmpset, NULL);
#endif
    }
    sigprocmask(SIG_SETMASK, &save_set, NULL); //无意义
    exit(0);
}
