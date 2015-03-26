/* testadd - Simple program to add two random matrices and
 * print the result
 *
 * Jerod Weinman
 * 25 June 2008
 */

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "matrixop.h"
#include "matrixutil.h"

int main(int argc, char* argv[])
{
     int rows, cols;        /* Dimensions read from user */
     struct matrix_t a,b,c; /* Matrix variables (two summands and a sum) */

     /* Check usage */
     if (argc!=3)
     {
          fprintf(stderr,"Usage: %s rows cols\n",argv[0]);
          exit (1);
     }

     rows = (int) strtol (argv[1], NULL, 10);
     cols = (int) strtol (argv[2], NULL, 10);

     if (rows<1 || cols<1)
     {
          fprintf(stderr,"%s: rows and cols must be positive",argv[0]);
          exit (1);
     }

     /* Allocate space for the three matrices */
     mtxAlloc(&a, rows, cols);
     mtxAlloc(&b, rows, cols);
     mtxAlloc(&c, rows, cols);

     /* Fill a and b with random values */
     mtxRand(&a);
     mtxRand(&b);

     /* Calculate their sum */
     mtxAdd(&a,&b,&c);

     /* Print a result */
     mtxPrint(&a);
     printf(" + \n");
     mtxPrint(&b);
     printf(" = \n");
     mtxPrint(&c);

     /* Free matrices' resources */
     mtxFree(&a);
     mtxFree(&b);
     mtxFree(&c);

     /* Indicate successful completion */
     exit (EXIT_SUCCESS);

}
