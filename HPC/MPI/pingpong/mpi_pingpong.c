#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
/* for memcpy */ 
#include <time.h> 
#include "mpi.h"

/* Use these parameters for generating times*/ 
#define RES_TEST_ITERS 10 
#define MAX_MESG_SIZE 131072 
void print_buffer(char mesg[], int mesg_size, int my_rank); 
double ping_pong(char mesg[], int mesg_size, int iters, MPI_Comm comm, int p, int my_rank);
 /*-------------------------------------------------------------------*/ 
int main(int argc, char* argv[]) { 
	int i; 
	double elapsed; 
	char message[MAX_MESG_SIZE]; 
	char c = 'B'; 
	MPI_Comm comm;
	int p;
	int my_rank;

	MPI_Init(&argc, &argv);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &my_rank);
	if (p != 2) {
		if(my_rank == 0) {
			fprintf(stderr, "Use two processes. \n");
		}
		MPI_Finalize();
		return 0;
	}

	if(my_rank == 0) 
		c = 'A';
	for(i = 0; i< MAX_MESG_SIZE;i++)
		message[i] = c;

	elapsed = ping_pong(message, 0, RES_TEST_ITERS, comm, p, my_rank);
    if(my_rank == 0) {
	    fprintf(stderr, "iter: %d, Min ping_ping = %8.5e, Clock tick = %8.5e\n", i, elapsed/(2*RES_TEST_ITERS), MPI_Wtick());
	}	

	MPI_Finalize();
	return 0;
}

/*------------------------------- ~@~T---------*/
double ping_pong(char mesg[], int mesg_size, int iters, MPI_Comm comm,
                int p, int my_rank){
        int i;
        MPI_Status status;
        double start;

        if (my_rank == 0) {
                start =MPI_Wtime();
                for(i=0; i<iters; i++){
                        MPI_Send(mesg, mesg_size, MPI_CHAR,1,0, comm);
                        MPI_Recv(mesg, mesg_size, MPI_CHAR,1,0, comm, &status);
                }
                return MPI_Wtime() - start;
        }else if(my_rank == 1){
                for (i= 0; i<iters; i++){
                        MPI_Recv(mesg, mesg_size,MPI_CHAR,0,0, comm, &status);
                        print_buffer(mesg, mesg_size,1);
                        MPI_Send(mesg, mesg_size, MPI_CHAR,0,0, comm);
                }
        }
        return 0.0;
}/* ping _pong */

/*-----------------------------------_*/
void print_buffer(char mesg[],int mesg_size,int my_rank){
        char temp[MAX_MESG_SIZE + 1];


        memcpy(temp, mesg, mesg_size);
        temp[mesg_size]='\0';
        //printf("Process %d > %s\n", my_rank, temp);
        fflush(stdout);
}
/*print buffer*/
