
/*****************************************************************************
Authors  : Albert Owusu-Asare , Box 4497, <owusuasa@grinnell.edu>
         : Name               , Box ,     <username@grinnell.edu>

This document contains answers to questions from CS213 Lab: Locatlity in
Matrix Multiplication



Questions:
 http://www.cs.grinnell.edu/~weinman/courses/CSC213/2014F/labs/memory.html

*****************************************************************************/


/*
 * This program was highly motivated be 'testadd.c' by Jerod Weinman
 * testmultiply - Simple program to mulitply two random matrices and
 * print the result
 *
 * see 'testadd.c' by :
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
           /* Dimensions read from user */
  int rows_a, cols_a;  
    int rows_b, cols_b;  
    int rows_c, cols_c;
     struct matrix_t a,b,c; /* Matrix variables (two summands and a sum) */

     /* Check usage */
     if (argc!=5)
     {
          fprintf(stderr,"Usage: %s rows cols\n",argv[0]);
          exit (1);
     }

     //input for matrix A rows and columns
     rows_a = (int) strtol (argv[1], NULL, 10);
     cols_a = (int) strtol (argv[2], NULL, 10);

     //input for matrix B rows and columns
    rows_b = (int) strtol (argv[3], NULL, 10);
     cols_b = (int) strtol (argv[4], NULL, 10);

     // set dimensions for matrix c rows and columns
     rows_c = rows_a;
     cols_c = cols_b;

     if (rows_a<1 || cols_a<1 || rows_b<1 || cols_b<1)
     {
          fprintf(stderr,"%s: rows and cols must be positive",argv[0]);
          exit (1);
     }

     /* Allocate space for the three matrices */
     mtxAlloc(&a, rows_a, cols_a);
     mtxAlloc(&b, rows_b, cols_b);
     mtxAlloc(&c, rows_c, cols_c);

     /* Fill a and b with random values */
     mtxRand(&a);
     mtxRand(&b);
      mtxPrint(&a); // testing purposes
      printf(" * \n"); // testing purposes
      mtxPrint(&b); //testing purposes

      //  mtxPrint(&c); //testing purposes


     /* Calculate their product */
     mtxMultiplyMin(&a,&b,&c);

     /* Print a result */
     mtxPrint(&a);
     printf(" * \n");
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
