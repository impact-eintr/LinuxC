#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    putchar('a');
    write(1, "b", 1);

    putchar('a');
    write(1, "b", 1);

    putchar('a');
    write(1, "b", 1);

    exit(1);
}
