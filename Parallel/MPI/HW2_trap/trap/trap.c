#include <stdio.h>
#include <stdlib.h>
#include "trap.h"


double Trap(double a,double b,int n,double h,double f(double)){
    double result;
    int k;

    result = (f(a) + f(b))/2.0;
    for(k = 1; k<=n-1; k++) {
        result += f(a + k*h);

    }
    result = result * h;
    return result;
}

