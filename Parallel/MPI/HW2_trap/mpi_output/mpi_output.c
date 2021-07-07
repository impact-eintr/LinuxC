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
