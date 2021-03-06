#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "trap.h"

static double f(double x){
    return sin(x);
}

void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
      int* n_p) {
   int dest;

   if (my_rank == 0) {
       printf("请输入a, b, n:\n");
       scanf("%lf",a_p);
       scanf("%lf",b_p);
       scanf("%d",n_p);
      for (dest = 1; dest < comm_sz; dest++) {
         MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
         MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
         MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      } 
   } else { /* my_rank != 0 */
      MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
      MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
      MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
   } 
}

//如何只输出一句提示
int main() {
    int my_rank, comm_sz, n , local_n;   
    double a , b , h, local_a, local_b;
    double local_int, total_int;
    int source;
    double remain = 0.0;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    Get_input(my_rank, comm_sz, &a, &b, &n);

    h = (b-a)/n;  
    local_n = n/comm_sz;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h,f);

    if (my_rank != 0) { 
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, 
                    MPI_COMM_WORLD); 
    } else {
        total_int = local_int;
        for (source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
        // 这里是因为有一些矩阵没有计算到，所以，非整除的情况下结果会有比较大的缺失。
        // 这里将这些矩形在0号线程上补上。
        if (n%comm_sz){
            remain = Trap(h * local_n * comm_sz, b, n%comm_sz, h, f);
        }

        total_int += remain;

    }

    if (my_rank == 0) {
        printf("当n = %d , 区间 (%f ,%f)的面积 = %.15e\n",
                    n ,a, b, total_int);
    }

    MPI_Finalize();

    return 0;
}
