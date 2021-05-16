#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "trap.h"

static double f(double x){
    return sin(x);
}

void Get_input(int my_rank, double* a_p, double* b_p,
            int* n_p) {

    if (my_rank == 0) {
        printf("请输入a, b, n:\n");
        scanf("%lf",a_p);
        scanf("%lf",b_p);
        scanf("%d",n_p);
    } 
    MPI_Bcast(a_p,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(b_p,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(n_p,1,MPI_INT,0,MPI_COMM_WORLD);
}

int main() {
    int my_rank, comm_sz, n , local_n;   
    double a , b , h, local_a, local_b;
    double local_int, total_int;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    Get_input(my_rank, &a, &b, &n);

    h = (b-a)/n;  
    local_n = n/comm_sz;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h,f);
    
    printf("[%d] a =%f  b = %f n = %d local_int = %f\n",my_rank, local_a, local_b, local_n, local_int);
    
    MPI_Allreduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    printf("当n = %d , 区间 (%f ,%f)的面积 = %.15e\n",
                n ,a, b, total_int);

    MPI_Finalize();

    return 0;
}
