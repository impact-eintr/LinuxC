# 实验3

## 编写奇偶排序串行代码
~~~ c
#include <stdio.h>
#include <stdlib.h>

/* Keys in the random list in the range 0 <= key < RMAX */
const int RMAX = 100;

void Usage(char* prog_name);
void Get_args(int argc, char* argv[], int* n_p, char* g_i_p);
void Generate_list(int a[], int n);
void Print_list(int a[], int n, char* title);
void Read_list(int a[], int n);
void Odd_even_sort(int a[], int n);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int  n;
   char g_i;
   int* a;

   Get_args(argc, argv, &n, &g_i);
   a = (int*) malloc(n*sizeof(int));
   if (g_i == 'g') {
      Generate_list(a, n);
      Print_list(a, n, "Before sort");
   } else {
      Read_list(a, n);
   }

   Odd_even_sort(a, n);

   Print_list(a, n, "After sort");
   
   free(a);
   return 0;
}  /* main */


void Usage(char* prog_name) {
   fprintf(stderr, "usage:   %s <n> <g|i>\n", prog_name);
   fprintf(stderr, "   n:   number of elements in list\n");
   fprintf(stderr, "  'g':  generate list using a random number generator\n");
   fprintf(stderr, "  'i':  user input list\n");
}  /* Usage */


void Get_args(int argc, char* argv[], int* n_p, char* g_i_p) {
   if (argc != 3 ) {
      Usage(argv[0]);
      exit(0);
   }
   *n_p = atoi(argv[1]);
   *g_i_p = argv[2][0];

   if (*n_p <= 0 || (*g_i_p != 'g' && *g_i_p != 'i') ) {
      Usage(argv[0]);
      exit(0);
   }
}  /* Get_args */



void Generate_list(int a[], int n) {
   int i;

   srandom(0);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
}  /* Generate_list */



void Print_list(int a[], int n, char* title) {
   int i;

   printf("%s:\n", title);
   for (i = 0; i < n; i++)
      printf("%d ", a[i]);
   printf("\n\n");
}  /* Print_list */


void Read_list(int a[], int n) {
   int i;

   printf("Please enter the elements of the list\n");
   for (i = 0; i < n; i++)
      scanf("%d", &a[i]);
}  /* Read_list */


void Odd_even_sort(
      int  a[]  /* in/out */, 
      int  n    /* in     */) {
   int phase, i, temp;

   for (phase = 0; phase < n; phase++) 
      if (phase % 2 == 0) { /* Even phase */
         for (i = 1; i < n; i += 2) 
            if (a[i-1] > a[i]) {
               temp = a[i];
               a[i] = a[i-1];
               a[i-1] = temp;
            }
      } else { /* Odd phase */
         for (i = 1; i < n-1; i += 2)
            if (a[i] > a[i+1]) {
               temp = a[i];
               a[i] = a[i+1];
               a[i+1] = temp;
            }
      }
}
~~~

![image-20210411165332973](/home/yixingwei/.config/Typora/typora-user-images/image-20210411165332973.png)

## 编写奇偶排序并行代码

~~~ c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

const int RMAX = 100;

/* Local functions */
void Usage(char* program);
void Print_list(int local_A[], int local_n, int rank);
void Merge_low(int local_A[], int temp_B[], int temp_C[], 
         int local_n);
void Merge_high(int local_A[], int temp_B[], int temp_C[], 
        int local_n);
void Generate_list(int local_A[], int local_n, int my_rank);
int  Compare(const void* a_p, const void* b_p);

/* Functions involving communication */
void Get_args(int argc, char* argv[], int* global_n_p, int* local_n_p, 
         char* gi_p, int my_rank, int p, MPI_Comm comm);
void Sort(int local_A[], int local_n, int my_rank, 
         int p, MPI_Comm comm);
void Odd_even_iter(int local_A[], int temp_B[], int temp_C[],
         int local_n, int phase, int even_partner, int odd_partner,
         int my_rank, int p, MPI_Comm comm);
void Print_local_lists(int local_A[], int local_n, 
         int my_rank, int p, MPI_Comm comm);
void Print_global_list(int local_A[], int local_n, int my_rank,
         int p, MPI_Comm comm);
void Read_list(int local_A[], int local_n, int my_rank, int p,
         MPI_Comm comm);


/*-------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int my_rank, p;
   char g_i;
   int *local_A;
   int global_n;
   int local_n;
   MPI_Comm comm;

   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &p);
   MPI_Comm_rank(comm, &my_rank);

   Get_args(argc, argv, &global_n, &local_n, &g_i, my_rank, p, comm);
   local_A = (int*) malloc(local_n*sizeof(int));
   if (g_i == 'g') {
      Generate_list(local_A, local_n, my_rank);
      Print_local_lists(local_A, local_n, my_rank, p, comm);
   } else {
      Read_list(local_A, local_n, my_rank, p, comm);
#     ifdef DEBUG
      Print_local_lists(local_A, local_n, my_rank, p, comm);
#     endif
   }

#  ifdef DEBUG
   printf("Proc %d > Before Sort\n", my_rank);
   fflush(stdout);
#  endif
   Sort(local_A, local_n, my_rank, p, comm);

#  ifdef DEBUG
   Print_local_lists(local_A, local_n, my_rank, p, comm);
   fflush(stdout);
#  endif

   Print_global_list(local_A, local_n, my_rank, p, comm);

   free(local_A);

   MPI_Finalize();

   return 0;
}  /* main */


void Generate_list(int local_A[], int local_n, int my_rank) {
   int i;

    srandom(my_rank+1);
    for (i = 0; i < local_n; i++)
       local_A[i] = random() % RMAX;

}  /* Generate_list */


void Usage(char* program) {
   fprintf(stderr, "usage:  mpirun -np <p> %s <g|i> <global_n>\n",
       program);
   fprintf(stderr, "   - p: the number of processes \n");
   fprintf(stderr, "   - g: generate random, distributed list\n");
   fprintf(stderr, "   - i: user will input list on process 0\n");
   fprintf(stderr, "   - global_n: number of elements in global list");
   fprintf(stderr, " (must be evenly divisible by p)\n");
   fflush(stderr);
}  /* Usage */


void Get_args(int argc, char* argv[], int* global_n_p, int* local_n_p, 
         char* gi_p, int my_rank, int p, MPI_Comm comm) {

   if (my_rank == 0) {
      if (argc != 3) {
         Usage(argv[0]);
         *global_n_p = -1;  /* Bad args, quit */
      } else {
         *gi_p = argv[1][0];
         if (*gi_p != 'g' && *gi_p != 'i') {
            Usage(argv[0]);
            *global_n_p = -1;  /* Bad args, quit */
         } else {
            *global_n_p = atoi(argv[2]);
            if (*global_n_p % p != 0) {
               Usage(argv[0]);
               *global_n_p = -1;
            }
         }
      }
   }  /* my_rank == 0 */

   MPI_Bcast(gi_p, 1, MPI_CHAR, 0, comm);
   MPI_Bcast(global_n_p, 1, MPI_INT, 0, comm);

   if (*global_n_p <= 0) {
      MPI_Finalize();
      exit(-1);
   }

   *local_n_p = *global_n_p/p;
#  ifdef DEBUG
   printf("Proc %d > gi = %c, global_n = %d, local_n = %d\n",
      my_rank, *gi_p, *global_n_p, *local_n_p);
   fflush(stdout);
#  endif

}  /* Get_args */


void Read_list(int local_A[], int local_n, int my_rank, int p,
         MPI_Comm comm) {
   int i;
   int *temp;

   if (my_rank == 0) {
      temp = (int*) malloc(p*local_n*sizeof(int));
      printf("Enter the elements of the list\n");
      for (i = 0; i < p*local_n; i++)
         scanf("%d", &temp[i]);
   } 

   MPI_Scatter(temp, local_n, MPI_INT, local_A, local_n, MPI_INT,
       0, comm);

   if (my_rank == 0)
      free(temp);
}  /* Read_list */


void Print_global_list(int local_A[], int local_n, int my_rank, int p, 
      MPI_Comm comm) {
   int* A;
   int i, n;

   if (my_rank == 0) {
      n = p*local_n;
      A = (int*) malloc(n*sizeof(int));
      MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0,
            comm);
      printf("Global list:\n");
      for (i = 0; i < n; i++)
         printf("%d ", A[i]);
      printf("\n\n");
      free(A);
   } else {
      MPI_Gather(local_A, local_n, MPI_INT, A, local_n, MPI_INT, 0,
            comm);
   }

}  /* Print_global_list */

int Compare(const void* a_p, const void* b_p) {
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else /* a > b */
      return 1;
}  /* Compare */

void Sort(int local_A[], int local_n, int my_rank, 
         int p, MPI_Comm comm) {
   int phase;
   int *temp_B, *temp_C;
   int even_partner;  /* phase is even or left-looking */
   int odd_partner;   /* phase is odd or right-looking */

   /* Temporary storage used in merge-split */
   temp_B = (int*) malloc(local_n*sizeof(int));
   temp_C = (int*) malloc(local_n*sizeof(int));

   /* Find partners:  negative rank => do nothing during phase */
   if (my_rank % 2 != 0) {
      even_partner = my_rank - 1;
      odd_partner = my_rank + 1;
      if (odd_partner == p) odd_partner = MPI_PROC_NULL;  // Idle during odd phase
   } else {
      even_partner = my_rank + 1;
      if (even_partner == p) even_partner = MPI_PROC_NULL;  // Idle during even phase
      odd_partner = my_rank-1;  
   }

   /* Sort local list using built-in quick sort */
   qsort(local_A, local_n, sizeof(int), Compare);

#  ifdef DEBUG
   printf("Proc %d > before loop in sort\n", my_rank);
   fflush(stdout);
#  endif

   for (phase = 0; phase < p; phase++)
      Odd_even_iter(local_A, temp_B, temp_C, local_n, phase, 
             even_partner, odd_partner, my_rank, p, comm);

   free(temp_B);
   free(temp_C);
}  /* Sort */


void Odd_even_iter(int local_A[], int temp_B[], int temp_C[],
        int local_n, int phase, int even_partner, int odd_partner,
        int my_rank, int p, MPI_Comm comm) {
   MPI_Status status;

   if (phase % 2 == 0) {
      if (even_partner >= 0) {
         MPI_Sendrecv(local_A, local_n, MPI_INT, even_partner, 0, 
            temp_B, local_n, MPI_INT, even_partner, 0, comm,
            &status);
         if (my_rank % 2 != 0)
            Merge_high(local_A, temp_B, temp_C, local_n);
         else
            Merge_low(local_A, temp_B, temp_C, local_n);
      }
   } else { /* odd phase */
      if (odd_partner >= 0) {
         MPI_Sendrecv(local_A, local_n, MPI_INT, odd_partner, 0, 
            temp_B, local_n, MPI_INT, odd_partner, 0, comm,
            &status);
         if (my_rank % 2 != 0)
            Merge_low(local_A, temp_B, temp_C, local_n);
         else
            Merge_high(local_A, temp_B, temp_C, local_n);
      }
   }
}  /* Odd_even_iter */


void Merge_low(
      int  my_keys[],     /* in/out    */
      int  recv_keys[],   /* in        */
      int  temp_keys[],   /* scratch   */
      int  local_n        /* = n/p, in */) {
   int m_i, r_i, t_i;
   
   m_i = r_i = t_i = 0;
   while (t_i < local_n) {
      if (my_keys[m_i] <= recv_keys[r_i]) {
         temp_keys[t_i] = my_keys[m_i];
         t_i++; m_i++;
      } else {
         temp_keys[t_i] = recv_keys[r_i];
         t_i++; r_i++;
      }
   }

   memcpy(my_keys, temp_keys, local_n*sizeof(int));
}  /* Merge_low */

void Merge_high(int local_A[], int temp_B[], int temp_C[], 
        int local_n) {
   int ai, bi, ci;
   
   ai = local_n-1;
   bi = local_n-1;
   ci = local_n-1;
   while (ci >= 0) {
      if (local_A[ai] >= temp_B[bi]) {
         temp_C[ci] = local_A[ai];
         ci--; ai--;
      } else {
         temp_C[ci] = temp_B[bi];
         ci--; bi--;
      }
   }

   memcpy(local_A, temp_C, local_n*sizeof(int));
}  /* Merge_high */


/*-------------------------------------------------------------------
 * Only called by process 0
 */
void Print_list(int local_A[], int local_n, int rank) {
   int i;
   printf("%d: ", rank);
   for (i = 0; i < local_n; i++)
      printf("%d ", local_A[i]);
   printf("\n");
}  /* Print_list */

void Print_local_lists(int local_A[], int local_n, 
         int my_rank, int p, MPI_Comm comm) {
   int*       A;
   int        q;
   MPI_Status status;

   if (my_rank == 0) {
      A = (int*) malloc(local_n*sizeof(int));
      Print_list(local_A, local_n, my_rank);
      for (q = 1; q < p; q++) {
         MPI_Recv(A, local_n, MPI_INT, q, 0, comm, &status);
         Print_list(A, local_n, q);
      }
      free(A);
   } else {
      MPI_Send(local_A, local_n, MPI_INT, 0, 0, comm);
   }
}
~~~

![image-20210411165244206](/home/yixingwei/.config/Typora/typora-user-images/image-20210411165244206.png)

## 课后习题

>  3.1
> 在问候程序中，如果strlen(greeting)代替strlen(greeting)+1来计算进程1、2、…、comm_sz-1发送消息的长度，会发生什么情况？如果用MAX_STRING代替strlen(greeting)+1又会是什么结果？你可以解释这些结果吗？

解答：
①这样的替换并不影响显示，不过传递的是两个不大一样的字符串。
【引用 58页】在我们的程序，参数msg_size是消息字符串加上C语言中字符串结束符\0所占的字符数量。参数msg_type的值是MPI_CHAR。这两个参数一起告知系统整个消息有strlen(geeting)+1个字符。
②这个问题与上面的问题完全相反。这里MAX_STRING是绝对大于strlen(greeting)+1的，所以传递过去的字符数量会是MAX_STRING，会将接收区的缓存占满。不过，对于打印输出来说，并没有什么影响。

>  3.2
> 改变梯形积分法，使其能够在comm_sz无法被n整除的情况下，正确估算积分值(假设n≥comm_sz)

~~~ c
#include <stdio.h>
#include <mpi.h>

double f(double x){
  return x*x;
}

double Trap(
  double left_endpt,   /*  in  */
  double right_endpt,  /*  in  */
  int trap_count,      /*  in  */
  double base_len      /*  in  */
  ){
  double estimate, x;
  int i;

  estimate = (f(left_endpt) + f(right_endpt))/2.0;
  for (i = 0; i < trap_count - 1; i++){
    x = left_endpt + (i + 1) * base_len;
    estimate += f(x);
  }
  return estimate * base_len;
}

int main(){
  int my_rank, comm_sz, n = 64, local_n;
  double a = 0.0, b = 3.0, h, local_a, local_b, remain = 0.0;
  double local_int, total_int;
  int source;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  //Get_data(my_rank, comm_sz, &a, &b, &n);

  h = (b - a)/n; //h is the same for all processes

  local_n = n/comm_sz; // So is the number of trapezoids

  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;
  local_int = Trap(local_a, local_b, local_n, h);



  if (my_rank != 0){
    MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  } else {

    total_int =local_int;
    for (source = 1; source < (comm_sz); source++){
      MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      total_int += local_int;
    }

    // 这里是因为有一些矩阵没有计算到，所以，非整除的情况下结果会有比较大的缺失。
    // 这里将这些矩形在0号线程上补上。
    if (n%comm_sz){
      remain = Trap(h * local_n * comm_sz, b, n%comm_sz, h);
    }
    
    total_int += remain;
  }

  if (my_rank == 0){
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
  }

  MPI_Finalize();
  return 0;
}


~~~

> 3.3
> 梯形积分法程序中那些变量是局部的，那些是全局的？

意，我们对标识符的选择，是为了区分局部变量与全局变量。局部变量只在使用它们的进程内有效。梯形积分法程序中的例子有：local_a,local_b和local_n。如果变量在所有进程中都有效，那么该变量就称为全局变量。该程序中的例子有：变量a,b和n。这与你在编程导论课上学到的用法不同。在编程导论课上，局部变量是指单个函数的私有变量，而全局变量是指所有函数都可以访问的变量。


>  3.4
> mpi_output.c程序中，每个进程只打印一行输出。修改程序，使程序能够按进程号的顺序打印，即，进程0先输出，然后进程1，一次类推。

~~~ c
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(){
  int my_rank, comm_sz, q;
  char message[100];

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  //printf("Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);

  if (my_rank != 0){
    sprintf(message, "Proc %d of %d > Does anyone have a toothpick?", my_rank, comm_sz);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    printf("Proc %d of %d > Does anyone have a toothpick??\n", my_rank, comm_sz);
    for(q = 1; q < comm_sz; q++){
      MPI_Recv(message, 100, MPI_CHAR, q, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", message);
    }
  }

  MPI_Finalize();
  return 0;
}

~~~

> 3.6
> 假设comm_sz=4，x是一个拥有n=14个元素的向量
> a 如何用块划分法在一个程序的进程间分发x的元素
> b 如何用循环划分法在一个程序的进程间分发x的元素
> c 如何用b=2大小的块用块-循环划分法在一个程序的进程间分发x的值
> 你的分配方法应该通用性足够强，无论comm_sz和n取何值都能分配。你应该使你的分配“公正”，如果q和r是任意的2个进程，分给q和r的x分量个数的差应尽可能小。
>

1. a 块划分

   ~~~ c
   进程0  | 0  1  2  3
   进程1  | 4  5  6  7
   进程2  | 8  9  10 11
   进程3  | 12 13
   
   ~~~

   

2. b 循环划分

	~~~ c
	进程0  | 0  4  8  12
	进程1  | 1  5  9  13
	进程2  | 2  6  10
	进程3  | 3  7  11

	~~~

3. b 循环划分

   ~~~ c
   进程0  | 0  1  8  9  
   进程1  | 2  3  10 11  
   进程2  | 4  5  12 13
   进程3  | 6  7
   
   ~~~

   

> 3.7
> 如果通信子只包含一个进程，不同的MPI集合通信函数分别会做什么。

所有的操作就相当于这个进程发送信息给自己，然后完成对应的操作。

> 3.8
> 假定comm_sz=8, n=16
> a 画一张图来说明进程0要分发n个元素的数组，怎样使用拥有comm_sz个进程的树形结构的通信来实现MPI_Scatter。
> b 画一张图来说明原先分布在comm_sz个进程间的n个数组元素由进程0保存，怎样使用树形结构的通信来实现MPI_Gather.



![scatter](/home/yixingwei/图片/scatter.jpg)



> 3.10![gatter](/home/yixingwei/图片/gatter.jpg)
>
> 程序3.9的Read_vector函数中，实际上使用了local_n作为MPI_Scatter的2个形式参数send_count和rece_count,为什么程序仍然能正常运行?

这种读取和分发输入向量的方法将只适用于块划分法，并且向量的分量个数可以整除comm_sz的情况