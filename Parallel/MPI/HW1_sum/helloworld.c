#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_STRING 100


int main()
{
    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);//使用8核
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    fprintf(stdout,"%d %d\n",my_rank,comm_sz);

    if (my_rank != 0){
        sprintf(greeting,"欢迎 [%d]进程 [%d]核心",my_rank,comm_sz);
        MPI_Send(greeting,MAX_STRING,MPI_CHAR,0,0,MPI_COMM_WORLD);
    }else {
        printf("欢迎 [%d]进程 [%d]核心 \n",my_rank,comm_sz);
        for (int i = 1;i < comm_sz;i++){
            MPI_Recv(greeting,MAX_STRING,MPI_CHAR,i,0,MPI_COMM_WORLD,NULL);
            printf("%s\n",greeting);
        }
    }
    MPI_Finalize();

    exit(0);
}

