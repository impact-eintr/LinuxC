#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM RIGHT - LEFT

struct prinum {
    int n;
};

static void* thr_primer(void* p)
{
    int i = ((struct prinum*)p)->n;
    int flag = 1;
    for (int j = 2; j < i / 2; j++) {
	if (i % j == 0) {
	    flag = 0;
	    break;
	}
    }
    if (flag)
	printf("%d is a primer\n", i);
    pthread_exit(p);
}

int main(void)
{
    int err;
    pthread_t tid[THRNUM];
    struct prinum* p;
    void* ptr;

    for (int i = LEFT; i <= RIGHT; i++) {
	p = malloc(sizeof(p));
	if (p == NULL) {
	    perror("mallloc()");
	    exit(1);
	}

	p->n = i;

	err = pthread_create(tid + (i - LEFT), NULL, thr_primer, p);
	if (err) {
	    fprintf(stderr, "pthread_create(:%s\n)", strerror(err));
	}
    }

    for (int i = LEFT; i <= RIGHT; i++) {
	pthread_join(tid[i - LEFT], &ptr);
	free(ptr);
    }
    exit(0);
}
