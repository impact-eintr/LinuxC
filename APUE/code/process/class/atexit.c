#include <stdio.h>
#include <stdlib.h>

void func1()
{
    printf("func1 is ok\n");
}

void func2()
{
    printf("func2 is ok\n");
}

int main(void)
{
    printf("start...\n");

    atexit(func1);
    atexit(func2);

    printf("end...\n");
    exit(0);
}
