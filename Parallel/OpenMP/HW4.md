# 实验四

> Pthread实验编译使用到的Makefile

~~~ makefile
CFLAGS		+=-Wall -g -lpthread -O2
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
~~~

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;  

void Usage(char* prog_name);
void *Hello(void* rank);  /* Thread function */

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 

   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          Hello, (void*) thread);  

   printf("Hello from the main thread\n");

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   free(thread_handles);
   return 0;
}  /* main */

void *Hello(void* rank) {
   long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 

   printf("Hello from thread %ld of %d\n", my_rank, thread_count);

   return NULL;
}  /* Hello */

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}
~~~


> OpenMP实验编译使用到的Makefile

~~~ makefile
CFLAGS		+=-Wall -g -lpthread -fopenmp
bin = ./bin/
src = $(wildcard *.c)
obj = $(patsubst %.c,%,$(src))

ALL:$(obj)

%:%.c
	gcc $< -o $(bin)$@ $(CFLAGS)

clean:
	-rm -i $(bin)*

.PHONY:clean ALL
~~~

## 梯形积分(错误检查)

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _OPENMP
# include <omp.h>
#endif

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
double Local_trap(double a, double b, int n);

int main(int argc, char* argv[]) {
   double  global_result;        /* Store result in global_result */
   double  a, b;                 /* Left and right endpoints      */
   int     n;                    /* Total number of trapezoids    */
   int     thread_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);
   if (n % thread_count != 0) Usage(argv[0]);

   global_result = 0.0;
#  pragma omp parallel num_threads(thread_count) 
   {
      double my_result = 0.0;
      my_result += Local_trap(a, b, n);
#     pragma omp critical
      global_result += my_result;
   }

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result);
   return 0;
}  /* main */

void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of trapezoids must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */

double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

double Local_trap(double a, double b, int n) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;

#ifdef _OPENMP
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();
#else
   int my_rank = 0;
   int thread_count = 1;
#endif

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 

   return my_result;
}
~~~




## 梯形积分(Local_trap)
~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _OPENMP
# include <omp.h>
#endif

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
double Local_trap(double a, double b, int n);

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  /* Store result in global_result */
   double  a, b;                 /* Left and right endpoints      */
   int     n;                    /* Total number of trapezoids    */
   int     thread_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);
   if (n % thread_count != 0) Usage(argv[0]);

#  pragma omp parallel num_threads(thread_count) reduction(+: global_result)
   global_result += Local_trap(a, b, n);

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result);
   return 0;
}  /* main */

void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of trapezoids must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */

double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

double Local_trap(double a, double b, int n) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
#ifdef _OPENMP
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();
#else
   int my_rank = 0;
   int thread_count = 1;
#endif

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 

   return my_result;
}
~~~



## 梯形积分(归约操作)

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
# include <omp.h>
#endif

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
    double  global_result = 0.0;
    double  a, b;               
    int     n;                  
    int     thread_count;

    if (argc != 2) Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    global_result = Trap(a, b, n, thread_count);

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n",
                a, b, global_result);
    return 0;
}  /* main */

void Usage(char* prog_name) {

    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    exit(0);
}  /* Usage */

double f(double x) {
    double return_val;

    return_val = x*x;
    return return_val;
}  /* f */

double Trap(double a, double b, int n, int thread_count) {
    double  h, approx;
    int  i;

    h = (b-a)/n; 
    approx = (f(a) + f(b))/2.0; 
#  pragma omp parallel for num_threads(thread_count) \
    reduction(+: approx)
    for (i = 1; i <= n-1; i++)
        approx += f(a + i*h);
    approx = h*approx; 

    return approx;
}
~~~


