/* Phineas Schlossberg & Ezra Edgerton
 * Box 4570
 * Lab 5: Locality
 * matrixop.c - Definition of some basic  matrix operations
 * Jerod Weinman
 * mtxMultiplyMin & mtxMultiplyMax & mtxCheckMultiplyDim
 * implimented by Phineas and Ezra
 * Code retrieved
 * http://www.cs.grinnell.edu/~weinman/courses/CSC213/2014F/labs/code/locality/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "matrixop.h"
#define SUB2IND(row,col,numcols) (row) * (numcols) + (col)
/* Check the dimensions of three matrices for the sum C=A+B 
 *
 * Returns -1 if there is a dimension mismatch, and zero otherwise. */
int mtxCheckAddDim(const struct matrix_t *a, const struct matrix_t *b, 
                        const struct matrix_t *c)
{
     
     if (a->rows != b->rows || a->rows != c->rows  ||
         a->cols != b->cols || a->cols != c->cols ) {
          return -1;
     }
     
     return 0;

}


/* Matrix addition C = A + B
 *
 * Preconditions:
 *   Matrix parameters a,b and c all have the same dimension
 *
 * Postconditions:
 *   The resulting matrix sum is stored in parameter c
 *   Return value of 0 indicates successful completion of the addition
 */
int mtxAdd( const struct matrix_t *a, const struct matrix_t *b, 
                   struct matrix_t *c)
{
     /* Make sure these matrices are "addable" */
     int res = mtxCheckAddDim(a,b,c);

     if (res<0) {
          fprintf(stderr,"Dimension mismatch for matrix add.\n");
          return res;
     }
     
     int m = a->rows;        /* Dimensions of input matrices */
     int n = a->cols;

     MTX_TYPE *pa, *pb, *pc; /* Pointers to matrix data */

     pa = a->data;           /* Initialize pointers to beginning of data */
     pb = b->data;
     pc = c->data;

     /* Iterate over every entry in the matrices, adding and storing the result */
     int i,j;
     for (i=0 ; i<m ; i++)
          for (j=0 ; j<n ; j++, pa++,pb++,pc++)
               *pc = *pa + *pb;

     return 0; /* Indicate successful completion */
}


/* Check the dimensions of three matrices for the product C = A * B 
 * The number of columns in the first matrix must = the number of rows
 * in the second matrix. The product matrix must have the same number of 
 * rows as  matrix A, and the same number of cols as matrix B
 *
 * Returns -1 if there is a dimension mismatch, and zero otherwise. */
int mtxCheckMultiplyDim( const struct matrix_t *a, 
                         const struct matrix_t *b,
                         const struct matrix_t *c)
{
  if (a->cols != b->rows || c->rows != a->rows || c-> cols != b->cols)
    return -1;
  else 
    return 0;
}


int mtxMultiplyMin( const struct matrix_t *a, 
                    const struct matrix_t *b,
                    struct matrix_t *c)
{
    /* Make sure these matrices are "multipliable" */
     int res = mtxCheckMultiplyDim(a,b,c);

     if (res<0) {
          fprintf(stderr,"Dimension mismatch for matrix multiply.\n");
          return res;
     }
     /* MTX_TYPE *pa = malloc( a->rows*a->cols * sizeof(MTX_TYPE) ); */
     /* MTX_TYPE *pb = malloc( b->rows*b->cols * sizeof(MTX_TYPE) );  */
     /* MTX_TYPE *pc = malloc( c->rows*c->cols * sizeof(MTX_TYPE) ); /\* Pointers to matrix data *\/ */

     MTX_TYPE *pa, *pb, *pc;
     pa = a->data;           /* Initialize pointers to beginning of data */
     pb = b->data;
     pc = c->data;
     int m = a->cols; //Get A columns
     int n = b->cols; // Get B cols
     int p = b->rows; // Get B rows
     int i, j, k;
     //BELOW Attempt at traversing matrix with pointer arithmetic
     for (i = 0; i<m-1; i++,(pc += c->cols), (pa += a->cols)){
       for (k = 0; k<n-1;k++, (pb += b->cols), pa++){
         for (j = 0;j<p-1;j++, pc++,pb++){
             *pc += *pa * *pb;
         }
       }
     }
     // BELOW: Attempt at traversing matrix without pointer arithmetic
     /* for (i = 0; i<m-1; i++){ */
     /*   for (k = 0; k<n-1;k++){ */
     /*     for (j = 0;j<p-1;j++){ */
     /*      pc[i][j] =pc[i][j] +  pa[i][k] * pb[k][j]; }}}*/

     return 0;
       
}

int mtxMultiplyMax( const struct matrix_t *a, 
                    const struct matrix_t *b,
                    struct matrix_t *c)

{
    /* Make sure these matrices are "multipliable" */
     int res = mtxCheckMultiplyDim(a,b,c);

     if (res<0) {
          fprintf(stderr,"Dimension mismatch for matrix multiply.\n");
          return res;
     }
          MTX_TYPE *pa, *pb, *pc; /* Pointers to matrix data */

     pa = a->data;           /* Initialize pointers to beginning of data */
     pb = b->data;
     pc = c->data;
     int m = a->cols; //Get A columns
     int n = b->cols; // Get B cols
     int p = b->rows; // Get B rows
     int i, j, k;
     //BELOW: Attempt at traversing array with pointer arithmetic
  for (j = 0;j<p;j++, pc++,pb++){     
       for (k = 0; k<n;k++, (pb += b->cols), pa++){
         for (i = 0; i<m; i++,(pc += c->cols), (pa += a->cols)){
           *pc += (*(pa) * *(pb) );
         }
       }
     }
 //BELOW: Attempt at traversing matrix without pointer arithmetic
     /* for (j = 0;j<p-1;j++){      */
     /*   for (k = 0; k<n-1;k++){ */
     /*     for (i = 0; i<m-1; i++){ */
     /*       pc[i][j] += pa[i][k] * pb[k][j]; */
     /*     } */
     /*   } */
     /* } */
   
     return 0;
       
}
