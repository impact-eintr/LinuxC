#include <stdio.h>
#include <stdlib.h>

void Get_dims(int* m_p, int* n_p);
void Read_matrix(char prompt[], double A[], int m, int n);
void Read_vector(char prompt [], double x[], int n);
void Print_matrix(char title[], double A[], int m, int n);
void Print_vector(char title[], double y[], int m);
void Mat_vect_mult(double A[], double x[], double y[], int m, int n);

/*-------------------------------------------------------------------*/
int main(void) {
   double* A = NULL;
   double* x = NULL;
   double* y = NULL;
   int m, n;

   Get_dims(&m, &n);
   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   if (A == NULL || x == NULL || y == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }
   Read_matrix("A", A, m, n);
#  ifdef DEBUG
   Print_matrix("A", A, m, n);
#  endif
   Read_vector("x", x, n);
#  ifdef DEBUG
   Print_vector("x", x, n);
#  endif

   Mat_vect_mult(A, x, y, m, n);

   Print_vector("y", y, m);

   free(A);
   free(x);
   free(y);
   return 0;
}  /* main */


void Get_dims(
              int*  m_p  /* out */, 
              int*  n_p  /* out */) {
   printf("Enter the number of rows\n");
   scanf("%d", m_p);
   printf("Enter the number of columns\n");
   scanf("%d", n_p);

   if (*m_p <= 0 || *n_p <= 0) {
      fprintf(stderr, "m and n must be positive\n");
      exit(-1);
   }
}  /* Get_dims */

void Read_matrix(
                 char    prompt[]  /* in  */, 
                 double  A[]       /* out */, 
                 int     m         /* in  */, 
                 int     n         /* in  */) {
   int i, j;

   printf("Enter the matrix %s\n", prompt);
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */

void Read_vector(
                 char    prompt[]  /* in  */, 
                 double  x[]       /* out */, 
                 int     n         /* in  */) {
   int i;

   printf("Enter the vector %s\n", prompt);
   for (i = 0; i < n; i++)
      scanf("%lf", &x[i]);
}  /* Read_vector */


void Print_matrix(
                  char    title[]  /* in */,
                  double  A[]      /* in */, 
                  int     m        /* in */, 
                  int     n        /* in */) {
   int i, j;

   printf("\nThe matrix %s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%f ", A[i*n+j]);
      printf("\n");
   }
}  /* Print_matrix */

void Print_vector(
                  char    title[]  /* in */, 
                  double  y[]      /* in */, 
                  int     m        /* in */) {
   int i;

   printf("\nThe vector %s\n", title);
   for (i = 0; i < m; i++)
      printf("%f ", y[i]);
   printf("\n");
}  /* Print_vector */


void Mat_vect_mult(
                   double  A[]  /* in  */, 
                   double  x[]  /* in  */, 
                   double  y[]  /* out */,
                   int     m    /* in  */, 
                   int     n    /* in  */) {
   int i, j;

   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i*n+j]*x[j];
   }
}  /* Mat_vect_mult */
