#include "mytbf.h"
#include <stdio.h>
#include <stdlib.h>

struct mytbf_st {
    int cps;
    int burst;
    int token;
    int pos;
};

typedef void (*sighandler_t)(int);

static struct mytbf_st* job[MYTBF_MAX];
static int inited = 0;
static struct sigaction alrm_sa_save;

static void alrm_action(int s, siginfo_t* infop, void* unused)
{
    if (infop->si_code != SI_KERNEL)
	return;
    for (int i = 0; i < MYTBF_MAX; i++) {
	if (job[i] != NULL) {
	    job[i]->token += job[i]->cps;
	    if (job[i]->token > job[i]->burst)
		job[i]->token = job[i]->burst;
	}
    }
}

static void moude_unload(void)
{
    for (int i = 0; i < MYTBF_MAX; i++)
	free(job[i]);
}

static void moude_load(void)
{
    struct sigaction sa;
    struct itimerval itv;

    sa.sa_sigaction = alrm_action;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGALRM, &sa, &alrm_sa_save);

    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, NULL);
    ;
    atexit(moude_unload);
}

static int get_free_pos(void)
{
    for (int i = 0; i < MYTBF_MAX; i++) {
	if (job[i] == NULL)
	    return i;
    }
    return -1;
}

mytbf_t* mytbf_init(int cps, int rst)
{
    struct mytbf_st* me;
    int pos;

    //init只能初始化一次令牌数组，因为alarm只能出现一次
    if (!inited) {
	moude_load();
	inited = 1;
    }

    get_free_pos();
    if (pos < 0)
	return NULL;

    me = malloc(sizeof(*me));
    if (me == NULL) {
	return NULL;
    }
    me->token = 0;
    me->cps = cps;
    me->burst = cps;
    me->pos = pos;

    job[pos] = me;

    return me;
}

int mytbf_fetchtoken(mytbf_t* ptr, int size)
{
    struct mytbf_st* me = ptr;
    if (size <= 0)
	return -EINVAL;

    while (me->token <= 0)
	pause();

    int n = me->token <= size ? me->token : size;
    me->token -= n;
    return n;
}

int mytbf_returntoken(mytbf_t* ptr, int size)
{
    struct mytbf_st* me = ptr;

    if (size <= 0)
	return -EINVAL;

    me->token += size;
    if (me->token > me->burst)
	me->token = me->burst;

    return size;
}

int mytbf_destrory(mytbf_t* ptr)
{
    struct mytbf_st* me = ptr;
    job[me->pos] = NULL;
    free(ptr);

    return 0;
}
