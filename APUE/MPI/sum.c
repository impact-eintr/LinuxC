#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10000

int main()
{

    long long res = 0;

    for (int i = 1;i < N;i++){
        res += i;
    }
    printf("%lld\n",res);
    exit(0);
}

