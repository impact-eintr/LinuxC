#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* To get pthread_getattr_np() declaration */
#endif
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error_en(en, msg)                                               \
	\
	do {                                                                         \
		errno = en;                                                                \
		perror(msg);                                                               \
		exit(EXIT_FAILURE);                                                        \
	} while (0)

static void *thread_start(void *arg) {
	int i, s;
	pthread_attr_t gattr;

	s = pthread_getattr_np(pthread_self(), &gattr);
	if (s != 0)
	  handle_error_en(s, "pthread_getattr_np");

	printf("Thread's detachstate attributes:\n");

	s = pthread_attr_getdetachstate(&gattr, &i);
	if (s)
	  handle_error_en(s, "pthread_attr_getdetachstate");
	printf("Detach state        = %s\n", (i == PTHREAD_CREATE_DETACHED)
				? "PTHREAD_CREATE_DETACHED"
				: (i == PTHREAD_CREATE_JOINABLE)
				? "PTHREAD_CREATE_JOINABLE"
				: "???");

	pthread_attr_destroy(&gattr);
}

int main(int argc, char *argv[]) {
	pthread_t thr;
	int s;

	s = pthread_create(&thr, NULL, &thread_start, NULL);
	if (s != 0) {
		handle_error_en(s, "pthread_create");
		return 0;
	}

	pthread_join(thr, NULL); //等待子线程结束
}
