#include <omp.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) 
{
    int N;
    scanf("%d",&N);
    int a[N];

    int thr_num = 0;
    thr_num = atoi(argv[1]);

    for (int i = 0;i < N;i++){
       scanf("%d",a+i);
    }

    int tmp = 0;
    int j = 1;

    for (int i = 0;i < N;i++){
        if (i % 2 == 0){
# pragma omp parallel for num_threads(thr_num)\
            default(none) shared(a, N) private(j, tmp)
            for (j = 1;j < N;j += 2) {
                if (a[j-1] > a[j]) {
                    tmp = a[j-1];
                    a[j-1] = a[j];
                    a[j] = tmp;
                }
            }

        }else{
# pragma omp parallel for num_threads(thr_num)\
            default(none) shared(a, N) private(j, tmp)
            for (j = 1;j < N-1;j += 2) {
                if (a[j] > a[j+1]) {
                    tmp = a[j+1];
                    a[j+1] = a[j];
                    a[j] = tmp;
                }
            }
        }
    }
    
    printf("a: ");
    for (int i = 0;i < N;i++){
       printf("%d ",*(a+i));
    }
    printf("\n");

}
