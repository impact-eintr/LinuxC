#include "anytime.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int job1, job2, job3;

    puts("Begin!");
    job1 = at_addjob(5, f1, "aaa");
    if (job1 < 0) {
        fprintf(stderr, "at_addjob():%s\n", strerror(-job1));
        exit(1);
    }
    at_addjob(2, f2, "bbb");
    at_addjob(7, f1, "ccc");
    puts("End!");

    while (1) {
    }
}
