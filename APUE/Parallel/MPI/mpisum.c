#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define N 10000

int main()
{
    int comm_sz;
    int my_rank;

    int *sum = malloc(sizeof(int));
    int res = 0;
    
    MPI_Init(NULL,NULL);
    //从这里开始并行执行
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);//使用4核
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if (my_rank != 0){
        int tempsum = 0;
        for (int i = my_rank;i < N;i+=comm_sz-1){
            tempsum += i;
        }
        *sum = tempsum;
        MPI_Send(sum,sizeof(*sum),MPI_INT,0,0,MPI_COMM_WORLD);
    }else {
        *sum = 0;
        for (int i = 1;i < comm_sz;i++){
            MPI_Recv(sum,sizeof(*sum),MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            res += *sum;
        }
        printf("%d\n",res);
    }
    MPI_Finalize();

    exit(0);
}

