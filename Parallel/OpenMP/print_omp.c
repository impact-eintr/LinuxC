#include <stdio.h>

int main()
{
#ifdef _OPENMP
    printf("_OPENMP:%d\n",_OPENMP);
#endif
    return 0;
}

