#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/

void *thread1(void *);
void *thread2(void *);

int i = 1;
int main(void)
{
	pthread_t t_a;
	pthread_t t_b;

	pthread_create(&t_a, NULL, thread2, (void *)NULL);//创建线程t_a
	pthread_create(&t_b, NULL, thread1, (void *)NULL); //创建线程t_b
	pthread_join(t_b, NULL);/*等待进程t_b结束*/
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	exit(0);
}

void *thread1(void *junk)
{
	for (i = 1; i <= 20; i++)
	{
		pthread_mutex_lock(&mutex);//锁住互斥量
		if (i % 3 == 0)
			pthread_cond_signal(&cond); //唤醒等待条件变量cond的线程
		else       
			printf("thead1:%d\n", i); //打印不能整除3的i
		pthread_mutex_unlock(&mutex);//解锁互斥量

		sleep(1);
	}

}

void *thread2(void *junk)
{
	while (i < 20)
	{
		pthread_mutex_lock(&mutex);

		if (i % 3 != 0)
			pthread_cond_wait(&cond, &mutex);//等待条件变量
		printf("------------thread2:%d\n", i); //打印能整除3的i
		pthread_mutex_unlock(&mutex);

		sleep(1);
		i++;
	}

}
