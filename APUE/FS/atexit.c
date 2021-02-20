#include <stdio.h>
#include <stdlib.h>

static void f1(void) {
    puts("f1()");
}

static void f2(void) {
    puts("f2()");
}

static void f3(void) {
    puts("f3()");
}

int main()
{
    puts("start!");
    atexit(f1);
    atexit(f2);
    atexit(f3);
    exit(0);
}

