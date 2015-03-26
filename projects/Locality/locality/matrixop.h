/* matrixop.h - Definition of some basic  matrix operations
 *
 * Created by Jerod Weinman, 21 May 2008
 * Modified by Albert Owusu-Asare , Oct 6. (Marked : MODIFIED)
 *
 * mtxAdd, Matrix addition C = A + B
 */
#ifndef __MATRIXOP_H__
#define __MATRIXOP_H__

#include "matrix.h"

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
                  struct matrix_t *c );



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

int mtxMultiplyMax( const struct matrix_t *a, const struct matrix_t *b, 
                 struct matrix_t *c);

int mtxMultiplyMin( const struct matrix_t *a, const struct matrix_t *b, 
                 struct matrix_t *c);
#endif
