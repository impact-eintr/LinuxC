#ifndef VECTOR_ADD_H__
#define VECTOR_ADD_H__

#include <mpi.h>

void Vector_sum(double [],double [],double [],int);
void Read_n(int* n_p);
void Allocate_vectors(double**  x_pp, double**  y_pp, double**  z_pp, int n);
void Read_vector(double  a[], int n, char vec_name[]);
void Print_vector(double  b[], int n, char title[]);

void Check_for_error(int local_ok, char fname[], char message[], 
      MPI_Comm comm);
void Get_dims(int* m_p, int* local_m_p, int* n_p, int* local_n_p,
      int my_rank, int comm_sz, MPI_Comm comm);
void Allocate_arrays(double** local_A_pp, double** local_x_pp, 
      double** local_y_pp, int local_m, int n, int local_n, 
      MPI_Comm comm);
void Read_matrix(char prompt[], double local_A[], int m, int local_m, 
      int n, int my_rank, MPI_Comm comm);
void Read_vector(char prompt[], double local_vec[], int n, int local_n, 
      int my_rank, MPI_Comm comm);
void Print_matrix(char title[], double local_A[], int m, int local_m, 
      int n, int my_rank, MPI_Comm comm);
void Print_vector(char title[], double local_vec[], int n,
      int local_n, int my_rank, MPI_Comm comm);
void Mat_vect_mult(double local_A[], double local_x[], 
      double local_y[], int local_m, int n, int local_n, 
      MPI_Comm comm);

/*-------------------------------------------------------------------*/

#endif
