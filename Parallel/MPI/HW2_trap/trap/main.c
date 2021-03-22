#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "trap.h"

static double f(double x){
    return sin(x);
}

int main() {
    double result;
    double a, b, h;
    int n;

    printf("请输入a, b, n:\n");
    scanf("%lf",&a);
    scanf("%lf",&b);
    scanf("%d",&n);

    h = (b-a)/n;
    result = Trap(a, b, n, h,f);

    printf("当n=%d时，从区间%.4f到%.4f估计值为%.8f\n", n, a, b, result);
    exit(0);

}
