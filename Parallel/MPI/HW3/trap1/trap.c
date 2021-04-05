#include <stdio.h>
#include "trap.h"

double Trap(double left_endpt,double right_endpt,int trap_count,double base_len,double f(double)) 
{
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

