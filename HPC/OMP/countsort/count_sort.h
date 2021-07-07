#ifndef COUNT_SORT_H__
#define COUNT_SORT_H__

void Usage(char prog_name[]);
void Get_args(char* argv[], int* thread_count_p, int* n_p);
void Gen_data(int a[], int n);
void Count_sort_serial(int a[], int n);
void Count_sort_parallel(int a[], int n, int thread_count); 
void Library_qsort(int a[], int n);
int My_compare(const void* a, const void* b);
void Print_data(int a[], int n, char msg[]);
int Check_sort(int a[], int n);


#endif
