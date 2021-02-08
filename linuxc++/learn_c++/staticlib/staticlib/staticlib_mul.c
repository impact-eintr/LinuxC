#include "apue.h"

int static_lib_func_mul(int i1, int i2)
{
    int iret = i1 * i2;
    printf("in a static library, return value is %d\n", iret);
    return iret;
}
