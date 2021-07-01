#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define RMAX 100

void Init_list(int *local_A, int n);
void Print_global_list(int *global_A, int global_n);
void Print_list(int *local_A, int local_n, int rank);
void Merge(int* Local_A, int* local_B, int* local_n_p);
void Generate_list(int *local_A, int local_n, int my_rank);

int Compare(const void *a_p, const void *b_p);

void Sort(int *local_A, int global_n, int *local_n_p, int my_rank, int p, MPI_Comm comm);
void Print_local_lists(int *local_A, int local_n, unsigned bitmask, int my_rank, int p, MPI_Comm comm);


int main(int argc, char** argv) {

  int my_rank, p; int *local_A; int global_n; int local_n;

  MPI_Comm comm;

  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &p);
  MPI_Comm_rank(comm, &my_rank);

  global_n = 8; //手动调整排序序列的个数local_n = global_n/p;
                //p需要整除选为2
  local_A = malloc(global_n*sizeof(int));
  Init_list(local_A, global_n);
  Generate_list(local_A, local_n, my_rank);

  Print_local_lists(local_A, local_n, 0, my_rank, p, comm);
  Sort(local_A, global_n, &local_n, my_rank, p, comm);

  if (my_rank == 0)
    Print_global_list(local_A, global_n);

  free(local_A); MPI_Finalize();
  exit(0);

}

void Init_list(int *A, int n) {
  for (int i = 0; i < n; i++)
    A[i] = -1;
}

void Generate_list(int *local_A, int local_n, int my_rank) {
  // 补充srandom设置随机数种子
  for (int i = 0; i < local_n; i++)
    local_A[i] = random() % RMAX;

}

void Print_global_list(int *A, int n) {
  printf("The list is:\n ");
  for (int i = 0; i < n; i++)
    printf("%d ", A[i]);
  printf("\n");

}

int Compare(const void* a_p, const void* b_p) {
  int a = *((int*)a_p); int b = *((int*)b_p);
  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else /* a > b */
    return 1;

}

/*
 *Function:	Sort
 *Purpose:	Sort local list, merge local list with other lists
 *using tree structured communication
 *Input args:	global_n, my_rank, p, comm
 *In/out args: local_A, local_n_p
 *Notes:	local_n_p tracks the size of local_A, which doubles
 *with each iteration
 */
void Sort(int local_A[], int global_n, int* local_n_p, int my_rank, int p, MPI_Comm comm) {
  int partner;
  int done = 0;
  unsigned bitmask = 1;
  MPI_Status status;

  /* Sort local list using built-in quick sort */
  qsort(local_A, *local_n_p, sizeof(int), Compare);
  while (!done && bitmask < p) {
    partner = my_rank ^ bitmask;
    printf("Proc %d > partner = %d, bitmask = %d\n", my_rank, partner, bitmask);

    if (my_rank < partner) {
      // 补充代码/*进程编号小的进程接收其它进程发来的数组，
      // 若原数组为local_A， 接收到的数组为local_B。
      // 然后将local_A 和local_B实现并归排序存放在local_A内
      Merge(local_A, local_A + *local_n_p, local_n_p);
      Print_local_lists(local_A, *local_n_p, bitmask, my_rank, p, comm);
      //补充代码：使用左移赋值运算符对bitmask进行更新，1->2->4->8

    } else {
      //补充代码/*发送数组*/ done = 1;

    }

  } /* while */

}/* Sort */



void Merge(int local_A[], int local_B[], int* local_n_p) {

  //实现两个数组的并归

} /* merge */



/*
 *Only called by process 0
 */

void Print_list(int local_A[], int local_n, int rank) { int i;

  printf("[%d] -> ", rank);
  for (i = 0; i < local_n; i++) printf("%d ", local_A[i]);
  printf("\n");

}/* Print_list */

void Print_local_lists(int local_A[], int local_n, unsigned bitmask, int my_rank, int p, MPI_Comm comm) {

// 打印每个进程中的局部数组，注意需要由进程0统一进行打印

}
