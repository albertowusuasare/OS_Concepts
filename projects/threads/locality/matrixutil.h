/* Library of utilities for allocating, copying, reading, and randomizing matrices
 *
 * Created by Jerod Weinman, August 2008
 *
 * mtxAlloc - Allocate data for a matrix of the given dimension
 * mtxFree - Free data for a matrix
 * mtxCopy - Copy data and dimensions from one matrix to another.
 * mtxPrint - Print a matrix to stdout
 * mtxRead - Read a matrix from an open file stream
 * mtxWrite - Write a matrix to an open file stream
 * mtxRand - Fill a matrix's data with random values 
 */
#ifndef __MATRIXUTIL_H__
#define __MATRIXUTIL_H__

#include "matrix.h"

/* Allocate data for a matrix of the given dimensions in the struct
 * pointed to by mtx
 * 
 * Preconditions:
 *  rows, cols are positive
 *  mtx points to a struct matrix_t
 *
 * Postconditions:
 *  Return value of 0 indicates a successful completion of the request
 */
int mtxAlloc( struct matrix_t *mtx, int rows, int cols);

/* Free data for a matrix in the struct pointed to by mtx
 * 
 * Preconditions:
 *  mtx points to a struct matrix_t
 *  mtx was passed to mtxAlloc, which returned 0
 *
 * Postconditions:
 *  mtx->data has been freed; size of mtx is set to 0x0
 */
void mtxFree(struct matrix_t *mtx);

/* Copy data and dimensions from one matrix to another. 
 *
 * Preconditions:
 *  dst, src point to valid struct matrix_t
 *  dst, src have valid data pointers
 *  dst has sufficient data space allocated to allow copying of src data
 */
void mtxCopy(struct matrix_t *dst, const struct matrix_t *src);

/* Print a matrix to stdout */
void mtxPrint(const struct matrix_t *mtx);

/* Read a matrix from an open file stream
 *
 * Preconditions:
 *  fp points to a valid file stream open for reading
 *  mtx points to a struct matrix_t
 *
 * Postconditions:
 *  Memory is allocated in mtx for data and its dimensions are set
 *  Return value 0 indicates a successful read
 */
int mtxRead(FILE * fp, struct matrix_t *mtx);

/* Write a matrix to an open file stream
 *
 * Preconditions:
 *  fp points to a valid file stream open for writing
 *  mtx points to a valid struct matrix_t (including data)
 *
 * Postconditions:
 *  Matrix dimensions and data are written to the file stream
 *  Return value 0 indicates a successful write
 */
int mtxWrite(FILE * fp, struct matrix_t *mtx);

/* Fill a matrix's data with random values 
 * 
 * Preconditions:
 *  mtx points to a struct matrix_t
 * 
 * Postconditions:
 *  Every element in matrix data is given a random value
 */
void mtxRand( struct matrix_t *mtx);

#endif
