# 梯形积分

## 串行程序
~~~ c
#ifndef TRAP_H__
#define TRAP_H__

double Trap(double a,double b,int n,double h,double f(double));

#endif
~~~

~~~ c
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

~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include "trap.h"

static double f(double x){
    return x * x;
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
~~~

~~~ makefile
CFLAGS += -Wall -g -O2

all:trap

trap:main.c trap.o
	gcc $^ -o $@ $(CFLAGS)

trap.o:trap.c
	gcc $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
~~~

~~~ bash
make
./trap
~~~

## 并行版本 1
~~~ c
#ifndef TRAP_H__
#define TRAP_H__

double Trap(double a,double b,int n,double h,double f(double));

#endif
~~~

~~~ c
#include <stdio.h>
#include "trap.h"

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len,double f(double)) 
{
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

~~~

~~~ c
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
    } 

    if (my_rank == 0) {
        printf("当n = %d , 区间 (%f ,%f)的面积 = %.15e\n",
                    n ,a, b, total_int);
    }

    MPI_Finalize();

    return 0;
}
~~~

~~~ makefile
CFLAGS += -Wall -g -lm -O2
CC = mpicc

all:trap

trap:main.c trap.o
	$(CC) $^ -o $@ $(CFLAGS)

trap.o:trap.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
~~~

~~~ bash
make
mpiexec --mca opal_warn_on_missing_libcuda 0  ./trap
~~~

## 并行版本 2 通配符
~~~ c
#ifndef TRAP_H__
#define TRAP_H__

double Trap(double a,double b,int n,double h,double f(double));

#endif
~~~

~~~ c
#include <stdio.h>
#include "trap.h"

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len,double f(double)) 
{
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

~~~

~~~ c
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

int main() {
    int my_rank, comm_sz, n , local_n;   
    double a , b , h, local_a, local_b;
    double local_int, total_int;
    int source; 

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
            MPI_Recv(&local_int, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    } 

    if (my_rank == 0) {
        printf("当n = %d , 区间 (%f ,%f)的面积 = %.15e\n",
                    n ,a, b, total_int);
    }

    MPI_Finalize();

    return 0;
}
~~~

~~~ makefile
CFLAGS += -Wall -g -lm -O2
CC = mpicc

all:trap

trap:main.c trap.o
	$(CC) $^ -o $@ $(CFLAGS)

trap.o:trap.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
~~~

~~~ bash
make
mpiexec --mca opal_warn_on_missing_libcuda 0  ./trap
~~~

## 并行版本 3 除不尽
~~~ c
#ifndef TRAP_H__
#define TRAP_H__

double Trap(double a,double b,int n,double h,double f(double));

#endif
~~~

~~~ c
#include <stdio.h>
#include "trap.h"

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len,double f(double)) 
{
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

~~~

~~~ c
#include <stdlib.h>
#include <mpich/mpi.h>
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
~~~

~~~ makefile
CFLAGS += -Wall -g -lm -O2
CC = mpicc

all:trap

trap:main.c trap.o
	$(CC) $^ -o $@ $(CFLAGS)

trap.o:trap.c
	$(CC) $^ -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o trap
~~~

~~~ bash
make
mpiexec --mca opal_warn_on_missing_libcuda 0  ./trap
~~~

## 顺序输出
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

int main(void) {
   int my_rank, comm_sz;

   int* num = malloc(sizeof(int));

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank != 0){
       *num = my_rank;
       MPI_Send(num,sizeof(int),MPI_INT,0,0,MPI_COMM_WORLD);
   }else{
       printf("Proc %d of %d > Does anyone have a toothpick?\n",
                   my_rank, comm_sz);
       for (int i = 1;i < comm_sz;i++){
           MPI_Recv(num,sizeof(int),MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
           printf("Proc %d of %d > Does anyone have a toothpick?\n",
                       *num, comm_sz);
       }
   }

   MPI_Finalize();

   exit(0);
}
~~~
