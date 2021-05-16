#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

double f(double x) {
    double return_val;

    return_val = x*x;
    return return_val;
}

double Trap(double*a ,double* b, int* n, double h)
{

    printf("%lf %lf %d %lf\n",*a, *b, *n, h);
    double x = 0;
    double res = (f(*a) + f(*b))/2.0;
    for (int i = 1;i < *n-1;i++){
        x = *a+i*h;
        res += f(x);
    }
    res = res * h;
    return res;

}

int main(int argc, char** argv)
{
    double global_result = 0.0;
    double a, b;
    int n;
    int thr_num;

    thr_num = atoi(argv[1]);
    scanf("%lf%lf%d", &a, &b, &n);

#   pragma omp parallel num_threads(thr_num)
    {
        double h, my_result;
        double local_a, local_b;
        int local_n;

        int my_rank = omp_get_thread_num();
        int thr_count = omp_get_num_threads();

        h = (b-a)/n;
        local_n = n/thr_count;
        local_a = a+my_rank * h * local_n;
        local_b = local_a + local_n * h;

        my_result = Trap(&local_a, &local_b, &local_n, h);

#pragma omp critical
        global_result += my_result;
    }

    printf("%lf\n", global_result);
}
