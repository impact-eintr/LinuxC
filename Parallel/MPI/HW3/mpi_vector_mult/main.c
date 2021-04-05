#include <stdio.h>
#include <stdlib.h>
#include "vector_add.h"

int main(void) {
   int n;
   double *x, *y, *z;

   Read_n(&n);
   Allocate_vectors(&x, &y, &z, n);
   
   Read_vector(x, n, "x");
   Read_vector(y, n, "y");
   
   Vector_sum(x, y, z, n);

   Print_vector(z, n, "The sum is");

   free(x);
   free(y);
   free(z);

   return 0;
}  /* main */
