#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>

#include "count_sort.h"
#include "time.h"


int main(int argc, char* argv[]) { 
  int n, thread_count;
  int *a, *copy; 
  double start, stop;

  /* Check and get command line args */ 
  if (argc != 3) 
  Usage(argv[0]); 
  Get_args(argv, &thread_count, &n);

  /* Allocate storage and generate data for a */ 
  a = malloc(n*sizeof(int));
  Gen_data(a, n);

  /* Allocate storage for copy */ 
  copy = malloc(n*sizeof(int));

  /* Serial count sort */ 
  memcpy(copy, a, n*sizeof(int)); 
  GET_TIME(start);
  Count_sort_serial(copy, n);
  GET_TIME(stop);
  if (!Check_sort(copy, n)) 
    printf("Serial sort failed\n");

  printf("Serial run time: %e\n\n", stop-start);

  /* Parallel count sort */
  memcpy(copy, a, n*sizeof(int)); 

  GET_TIME(start);
  Count_sort_parallel(copy, n, thread_count); 
  GET_TIME(stop);

  if (!Check_sort(copy, n)) 
    printf("Parallel sort failed\n");
  printf("Parallel run time: %e\n\n", stop-start);

  /* qsort library */
  memcpy(copy, a, n*sizeof(int)); 

  GET_TIME(start); 
  Library_qsort(copy, n);
  GET_TIME(stop);

  Print_data(copy, n, "Sorted: Library qsort a");
  if (!Check_sort(copy, n))
    printf("Library sort failed\n"); 
  printf("qsort run time: %e\n", stop-start);

  free(a);
  free(copy);

  return 0;
} /* main */

void Usage(char prog_name[]){
  fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);
  exit(0);
}

void Get_args(char* argv[], int* thread_count_p, int* n_p) {
  *thread_count_p = strtol(argv[1], NULL, 10);
  *n_p = strtol(argv[2], NULL, 10);
} /* Get_args */

void Gen_data(int a[], int n) {
  int i;
  for (i = 0; i < n; i++)
  a[i] = random() % n + 1; // (double) RAND_MAX;

#  ifdef DEBUG
  Print_data(a, n, "a");
# endif
} /* Gen_data */

void Count_sort_serial(int a[], int n) {
  int i, j, count;
  int* temp = malloc(n*sizeof(int));

  for (i = 0; i < n; i++) { count = 0;
    for (j = 0; j < n; j++) if (a[j] < a[i])
    count++;
    else if (a[j] == a[i] && j < i) count++;
    temp[count] = a[i];
  }

  memcpy(a, temp, n*sizeof(int)); free(temp);
} /* Count_sort_serial */

void Print_data(int a[], int n, char msg[]) { int i;
  printf("%s = ", msg); 
  for (i = 0; i < n; i++)
    printf("%d ", a[i]);
  printf("\n");
} /* Print_data */

void Count_sort_parallel(int a[], int n, int thread_count) {
  int i, j, count;
  int* temp = malloc(n*sizeof(int));

# pragma omp parallel num_threads(thread_count) default(none) private(i, j, count) shared(n, a, temp)
  {
# ifdef DEBUG
    int my_rank = omp_get_thread_num(); 
# endif

# pragma omp for
    for (i = 0; i < n; i++) { 
      count = 0;
      for (j = 0; j < n; j++) 
      if (a[j] < a[i])
      count++;
      else if (a[j] == a[i] && j < i) 
      count++;

      temp[count] = a[i];
# ifdef DEBUG
      printf("Th %d > a[%d] = %d, count = %d\n", my_rank, i, a[i], count);
# endif
    }
    /* Note that there's an implicit barrier here */

# pragma omp for
    for (i = 0; i < n; i++) 
      a[i] = temp[i];

  } /* pragma omp parallel */

  memcpy(a, temp, n*sizeof(int)); free(temp);
} /* Count_sort_parallel */


void Library_qsort(int a[], int n) {
  qsort(a, n, sizeof(int), My_compare);
}

int My_compare(const void *a, const void *b){
  const int *int_a = (const int*)a;
  const int *int_b = (const int*)b;

  return (*int_a - *int_b);
}

int Check_sort(int a[], int n) {
  int i;
  for (i = 1; i < n; i++)
    if (a[i-1] > a[i])
      return 0;
  return 1;
}
