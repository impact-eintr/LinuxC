#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define  MAX_MESG_SIZE 1024*1024
#define  RES_TEST_ITERS 10

double ping_pong(char [], int, int, MPI_Comm, int, int);
void print_buf(char [], int, int );

int main(int argc,char **argv){
  double elapsed;
  char message[MAX_MESG_SIZE];
  char c = 'B';
  MPI_Comm comm;
  int comm_size;
  int my_rank;

  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  
  MPI_Comm_size(comm, &comm_size);
  MPI_Comm_rank(comm, &my_rank);
  if (comm_size != 2){
    if (my_rank == 0){
      fprintf(stderr, "Please Use Tow process\n");
    }
    MPI_Finalize();
    return 0;
  }

  if (my_rank == 0){
    c = 'A';
  }

  for (int i = 0;i < MAX_MESG_SIZE;i++){
    message[i] = c;
  }

  elapsed = ping_pong(message, 1024*1024, RES_TEST_ITERS, comm, comm_size, my_rank);
  if (my_rank == 0){
    fprintf(stderr, "Min ping_pong = %8.5e, Clock tick = %8.5e\n",
            elapsed/(2 * RES_TEST_ITERS), MPI_Wtick());
  }
  MPI_Finalize();
  exit(0);

}

double ping_pong(char *msg, int msg_size, int iters, MPI_Comm comm, int comm_size, int my_rank){
  MPI_Status status;
  double start;

  if (my_rank == 0){
    start = MPI_Wtime();
    for (int i = 0;i < iters;i++){
      MPI_Send(msg, msg_size, MPI_CHAR, 1, 0, comm);
      MPI_Recv(msg, msg_size, MPI_CHAR, 1, 0, comm, &status);
    }
    return MPI_Wtime() - start;
  }else if (my_rank == 1){
    for (int i = 0;i < iters;i++){
      MPI_Recv(msg, msg_size, MPI_CHAR, 0, 0, comm, &status);
      //print_buf(msg, msg_size, 1);
      MPI_Send(msg, msg_size, MPI_CHAR, 0, 0, comm);
    }
  }

  return 0.0;
}

void print_buf(char msg[], int msg_size,int my_rank){
  char temp[MAX_MESG_SIZE + 1];

  memcpy(temp, msg, msg_size);
  temp[msg_size] = '\0';
  printf("[%d] > %s\n",my_rank, temp);
  fflush(stdout);

}
