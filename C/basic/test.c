#include <stdio.h>

int main()
{
    int a = 1;
    double b = 2;
    while(a <= 128){
        b = a*b;
        a++;
        printf("b = %0.f\n",b);
    }
    return 0;
}

