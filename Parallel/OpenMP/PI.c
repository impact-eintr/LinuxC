#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
    int thr_num = 8;
    long long n = 10000000;
    double sum = 0.0;

    int i = 0;
    double flag = 0;

# pragma omp parallel for num_threads(thr_num) reduction(+: sum) private(flag)
    for (i = 0;i < n;i++) {
        if (flag%2 == 0) {
            flag = 1.0;
        sum += flag/(2*i+1);
        }else{

            flag = -1.0;
        sum += flag/(2*i+1);
        }
        printf("[%d] -> %d\n", omp_get_thread_num(), i);
    }

    printf("%.14f", 4 * sum);

}
