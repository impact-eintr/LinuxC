#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THRNUM 20
#define LINESIZE 1024
#define FNAME "./log"

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void* thr_add(void* p)
{
    FILE* fp;
    char linebuf[LINESIZE];

    fp = fopen(FNAME, "r+");
    if (fp == NULL) {
	perror("fopen()");
	exit(1);
    }

    pthread_mutex_lock(&mut);
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", atoi(linebuf) + 1);
    fclose(fp);
    pthread_mutex_unlock(&mut);

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid[THRNUM];
    int err;

    for (int i = 0; i < THRNUM; i++) {
	err = pthread_create(tid + i, NULL, thr_add, NULL);
	if (err) {
	    fprintf(stderr, "pthread_create():%s\n", strerror(err));
	    exit(1);
	}
    }

    for (int i = 0; i < THRNUM; i++)
	pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&mut);
}
