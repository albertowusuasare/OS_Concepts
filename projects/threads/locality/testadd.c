/* testadd - Simple program to add two random matrices and
 * print the result
 *
 * Created by Jerod Weinman, 25 June 2008
 * Revised by Jerod Weinman, 4 October 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "matrixop.h"
#include "matrixutil.h"

/* Parse command line parameters for usage:
 *  testadd rows cols
 * where rows,cols>0.
 *
 * In addition, parameters rows and cols must point to valid integers. 
 *
 * This procedure exits the program with status EXIT_FAILURE if these
 * preconditions are violated, otherwise sets *rows to rows and *cols
 * to cols
 */
void parseParameters(int argc, char* argv[], 
		     int* rows, int* cols)
{
  char* endPtr;          /* Temporary parsing check variable */

  /* Check usage */
  if (argc!=3)
  {
    fprintf(stderr,"Usage: %s rows cols\n",argv[0]);
    exit (EXIT_FAILURE);
  }
  
  *rows = (int) strtol (argv[1], &endPtr, 10);
  
  if ( (endPtr-argv[1])!=strlen(argv[1]) || /* Failed to parse an integer */
       *rows<=0 )/* Must be a positive integer */
  {
    fprintf(stderr,"%s: First parameter rows must be a positive integer\n", 
	    argv[0]);
    exit(EXIT_FAILURE);
  }
  
  *cols = (int) strtol (argv[2], &endPtr, 10);
  
  if ( (endPtr-argv[2])!=strlen(argv[2]) || /* Failed to parse an integer */
       *cols<=0 )/* Must be a positive integer */
  {
    fprintf(stderr,"%s: Second parameter cols must be a positive integer\n", 
	    argv[0]);
    exit(EXIT_FAILURE);
  }
}

/* Usage: testadd rows cols 
 *
 * Adds two matrices (containing random values) of the specified
 * dimensions, printing the addends and sum 
 */
int main(int argc, char* argv[])
{
     int rows, cols;        /* Dimensions read from user */
     struct matrix_t a,b,c; /* Matrix variables (two summands and a sum) */

     parseParameters(argc, argv, &rows, &cols);

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
