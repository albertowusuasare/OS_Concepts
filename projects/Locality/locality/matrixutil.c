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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#include "matrix.h"

#include "matrixutil.h"

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
int mtxAlloc(struct matrix_t *mtx, const int rows, const int cols)
{
  mtx->data = malloc( rows*cols * sizeof(MTX_TYPE) );

  if (mtx->data==NULL)  /* Allocation failed */
  {
    perror("Allocation for matrix failed");
    return -1;
  }

  /* Set dimensions */
  mtx->rows = rows;
  mtx->cols = cols;

  return 0;
}

/* Free data for a matrix in the struct pointed to by mtx
 * 
 * Preconditions:
 *  mtx points to a struct matrix_t
 *  mtx was passed to mtxAlloc, which returned 0
 *
 * Postconditions:
 *  mtx->data has been freed; size of mtx is set to 0x0
 */
void mtxFree(struct matrix_t *mtx)
{
  free(mtx->data);

  /* Set dimensions to empty matrix */
  mtx->rows = 0;
  mtx->cols = 0;
}

/* Copy data and dimensions from one matrix to another. 
 *
 * Preconditions:
 *  dst, src point to valid struct matrix_t
 *  dst, src have valid data pointers
 *  dst has sufficient data space allocated to allow copying of src data
 */
void mtxCopy(struct matrix_t *dst, const struct matrix_t *src)
{
  /* Copy data and dimensions */
  memcpy(dst->data,src->data, src->rows*src->cols*sizeof(MTX_TYPE)); 
  dst->rows = src->rows;
  dst->cols = src->cols;
}


/* Print a matrix to stdout */
void mtxPrint(const struct matrix_t *mtx)
{
  MTX_TYPE *pd = mtx->data; /* Pointer to matrix data */

  int i,j;                  /* Loop variables */
  int m = mtx->rows;          /* Matrix dimensions */
  int n = mtx->cols;

  for (i=0 ; i<m ; i++)
  {
    for (j=0 ; j<n ; j++,pd++)
      printf("%ld\t",*pd); //NB: Hard-coded format string must agree with MTX_TYPE
    printf("\n");
  }
}


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
int mtxRead(FILE * fp, struct matrix_t *mtx)
{
  size_t res;
  unsigned short rows, cols;
  int len;

  res = fread(&rows,sizeof(rows),1,fp); /* Read rows */

  if (res<1)
  {
    perror("Error reading number of rows");
    return -1;
  }
     
  res = fread(&cols,sizeof(cols),1,fp); /* Read cols */

  if (res<1)
  {
    perror("Error reading number of columns");
    return -1;
  }
     
  len = rows * cols; /* Total number of elements in the matrix */

  /* Try to allocate space for the data */
  res = mtxAlloc(mtx,rows,cols);

  if (res<0)
    return res;

  res = fread(mtx->data, sizeof(MTX_TYPE), len, fp); /* Read data */

  if (res < len) /* Insufficient data read ... end of file? read error? */
  {
    mtxFree(mtx); /* Free data and set size to 0 */
    perror("Error reading matrix data");
    return -1;
  }
     
  return 0;
}

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
int mtxWrite(FILE * fp, struct matrix_t *mtx)
{
  size_t res;
  unsigned short rows, cols;
  int len;

  rows = mtx->rows;
  res = fwrite(&rows,sizeof(rows),1,fp); /* Write rows */

  if (res<1)
  {
    perror("Error writing number of rows");
    return -1;
  }
     
  cols = mtx->cols;
  res = fwrite(&cols,sizeof(cols),1,fp); /* Write cols */

  if (res<1)
  {
    perror("Error writing number of columns");
    return -1;
  }
     
  len = rows * cols; // Total number of elements in the matrix

  res = fwrite(mtx->data, sizeof(MTX_TYPE), len, fp); // Write data

  if (res < len) /* Insufficient data written ... end of file? read error? */
  {
    perror("Error writing matrix data");
    return -1;
  }

  return 0;
}

/* Fill a matrix's data with random values 
 * 
 * Preconditions:
 *  mtx points to a struct matrix_t
 * 
 * Postconditions:
 *  Every element in matrix data is given a random value in [0,100)
 */
void mtxRand( struct matrix_t *mtx)
{
  int len = mtx->rows * mtx->cols; /* Total number of data elements */
  int i;
  MTX_TYPE *pd = mtx->data;

  for (i=0; i<len ; i++, pd++) /* Pointer-based loop over data locations */
    *pd = (MTX_TYPE)rand()%100; /* Random int */
}
