#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    for (int i = 0;i < 1025;i++){
        putchar('a');
        write(1,"b",1);
    }

    return 0;
}

