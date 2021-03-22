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

    int *task = malloc(sizeof(int));
    int *sum = malloc(sizeof(int));
    int res = 0;//最终结果
    
    MPI_Init(NULL,NULL);
    //从这里开始并行执行
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);//使用4核
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if (my_rank != 0){
        MPI_Recv(task,sizeof(*task),MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        int tempsum = 0;
        printf("task = %d\n",*task);
        for (int i = *task;i < *task+(N/(comm_sz-1));i++){
            tempsum += arr[i];
        }
        *sum = tempsum;
        MPI_Send(sum,sizeof(*sum),MPI_INT,0,0,MPI_COMM_WORLD);
    }else {
        *task = 0;
        for (int i = 0;i < comm_sz;i++){
            MPI_Send(task,sizeof(*task),MPI_INT,i,0,MPI_COMM_WORLD);
            *task = i*N/(comm_sz-1);
        }

        for (int i = 0;i < comm_sz;i++){
            MPI_Recv(sum,sizeof(*sum),MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            res += *sum;
        }
        
        printf("sum = %d\n",res);
    }
    MPI_Finalize();

    exit(0);
}

