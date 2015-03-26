/* matrix.h - Definition of a struct matrix_t type
 *
 * Jerod Weinman
 * 21 May 2008
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__

#define MTX_TYPE unsigned long // Type of data in the matrix

/* A simple matrix type that stores its data in a single linear array
 * using ROW-MAJOR ordering */ 
struct matrix_t {
     MTX_TYPE *data; /* Pointer to the data array */
     int rows;       /* Dimensions of the matrix */
     int cols;
};

#endif
