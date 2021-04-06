#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define N 24

static int arr[N] = {1,4,3,9,2,8,5,1,1,6,2,7,2,5,0,4,1,8,6,5,1,2,3,9};

int main()
{
    int comm_sz;
    int my_rank;

    int task;
    int *sum = malloc(sizeof(int));
    int *localsum = malloc(sizeof(int));

    MPI_Init(NULL,NULL);
    //从这里开始并行执行
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    task = my_rank * N/comm_sz;

    for (int i = task;i < task+(N/comm_sz);i++){
        *localsum += arr[i];
    }

    //MPI_Allreduce(localsum, sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Reduce(localsum, sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("sum = %d\n",*sum);

    MPI_Finalize();

    exit(0);
}

