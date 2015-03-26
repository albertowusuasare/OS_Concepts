/**
 * This document was modified
 * Authors : Albert Owusu-Asare, Kyle Morrehead
 * Original authors are listed below/
 * modified code has the MODIFIED keyword
 */

/* matrixop.c - Define some basic matrix operations 
 *
 * Jerod Weinman
 * 21 May 2008
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
                   struct matrix_t *c) {
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



/* MODIFIED : This is an additional piece to the original
 * Check the dimensions of three matrices for the sum C=AB 
 *
 * Returns -1 if there is a dimension mismatch, and zero otherwise. */

int mtxCheckMultiplyDim(const struct matrix_t *a, const struct matrix_t *b, 
                        const struct matrix_t *c)
{
     
     if (a->cols != b->rows || a->rows != c->rows 
         || b->cols != c->cols ) {
          return -1;
     }
     
     return 0;

}



/*
 * MODIFIED
 * Matrix multiplication C = AB
 *
 * Preconditions:
 *   Matrix parameters a,b,c have the required dimenetion.
    mtxCheckMultiplyDim(a,b,c) returns >0
 *
 * Postconditions:
 *   The resulting matrix sum is stored in parameter c
 *   Return value of 0 indicates successful completion of the multiplication
 */

int mtxMultiplyMin( const struct matrix_t *a, const struct matrix_t *b,
                   struct matrix_t *c)
{
     /* Make sure these matrices are "multiply-able" */
     int res = mtxCheckMultiplyDim(a,b,c);

     if (res<0) {
          fprintf(stderr,"Dimension mismatch for matrix add.\n");
          return res;
     }
     
     int m = a->rows;        /* Dimensions of input matrices */
     int n = a->cols;
     int p = b->cols;
  

     MTX_TYPE *pa, *pb, *pc; /* Pointers to matrix data */

     pa = a->data;           /* Initialize pointers to beginning of data */
     pb = b->data;
     pc = c->data;

     //  MTX_TYPE *tempa, *tempb; /* Pointers to matrix data */
     // tempa = pa  ;         /* Initialize pointers to beginning of data */
     //  tempb= pb;
     
     
     /* Iterate over every entry in the matrices, adding and storing the result */
     int i,j,k, l;

     /* initialise elements of matrix c to 0*/

     for (l = 0; l < (m*p); l++, pc++)
       {
         *pc = 0;
       }
     pc = c->data; // point back to start of c matrix

     // reference: #define SUB2IND(row,col,numcols) (row) * (numcols) + (col)
     /*
  for (j = 0;j<p;j++, pc++,pb++){     
       for (k = 0; k<n;k++, (pb += b->cols), pa++){
         for (i = 0; i<m; i++,(pc += c->cols), (pa += a->cols)){
           *pc += (*(pa) * *(pb) );
         }
       }
       }*/

     /*  for (i = 0; i < m; i++, pc = c->data + SUB2IND(i, 0, p), pb = b->data + SUB2IND(0, 0, p))
       //i loops through the rows of a and the rows of c
       {
         for (k = 0; k < n; k++, pa++, pc = c->data + SUB2IND(i, 0, p))
           //k loops through the columns of a and the rows of b
           {
             for (j = 0; j < p; j++,pb++,pc++)              
               //the columns of b and the columns of c
               {
                 *pc += *pa * *pb;
                
               }
           }  
           }*/

     for (i = 0; i < m; i++, pc +=SUB2IND(1, 0, p), pb = b->data)
       //i loops through the rows of a and the rows of c
       {
         for (k = 0; k < n; k++, pa++, pc = pc - p)
           //k loops through the columns of a and the rows of b
           {
             for (j = 0; j < p; j++,pb++,pc++)              
               //the columns of b and the columns of c
               {
                 *pc += *pa * *pb;
                
               }
           }  
       }

     return 0; /* Indicate successful completion */
}
int mtxMultiplyMax( const struct matrix_t *a, const struct matrix_t *b,
                   struct matrix_t *c)
{
     /* Make sure these matrices are "multiply-able" */
     int res = mtxCheckMultiplyDim(a,b,c);

     if (res<0) {
          fprintf(stderr,"Dimension mismatch for matrix add.\n");
          return res;
     }
     
     int m = a->rows;        /* Dimensions of input matrices */
     int n = a->cols;
     int p = b->cols;
  

     MTX_TYPE *pa, *pb, *pc; /* Pointers to matrix data */

     pa = a->data;           /* Initialize pointers to beginning of data */
     pb = b->data;
     pc = c->data;

     //  MTX_TYPE *tempa, *tempb; /* Pointers to matrix data */
     // tempa = pa  ;         /* Initialize pointers to beginning of data */
     //  tempb= pb;
     
     
     /* Iterate over every entry in the matrices, adding and storing the result */
     int i,j,k, l;

     /* initialise elements of matrix c to 0*/

     for (l = 0; l < (m*p); l++, pc++)
       {
         *pc = 0;
       }
     pc = c->data; // point back to start of c matrix

     // reference: #define SUB2IND(row,col,numcols) (row) * (numcols) + (col)
     /*
  for (j = 0;j<p;j++, pc++,pb++){     
       for (k = 0; k<n;k++, (pb += b->cols), pa++){
         for (i = 0; i<m; i++,(pc += c->cols), (pa += a->cols)){
           *pc += (*(pa) * *(pb) );
         }
       }
       }*/
     for (j = 0; j < p-1; i++, pa++, pc++)
       {
         for (k = 0; k < n-1; k++, pa++, pb++)
           {
             for (i = 0; i < m-1; j++, pc++, pb++)
               {
                 *pc += *pa * *pb;
               }
           }
       }

   return 0; /* Indicate successful completion */
}
