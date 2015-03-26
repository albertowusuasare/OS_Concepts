/* cubitmap
 * 
 * Routines for allocating, freeing, and moving the bitmap image
 * format to and from CUDA devices
 *
 * Created by Jerod Weinman, 13 August 2012
 * Revised 13 August 2014
 *
 * cu_bmalloc - Allocate memory for a bitmap on the CUDA device
 * hd_bmcopy - Copy the bitmap data from host bm_t to device memory
 * dh_bmcopy - Copy the bitmap data from device memory to host bm_t.
 * dd_bmcopy - Copy the bitmap data across device memory
 */

#include "cubitmap.h"


/* Allocate memory for a bm_t on the CUDA device */
bit* cu_bmalloc (int rows, int cols)
{
  
  bit* bits;
  
  cudaError_t rc = cudaMalloc( (void**)&bits,  cols * rows * sizeof(bit) );

  if (rc != cudaSuccess) {
    fprintf(stderr,"Unable to allocate space on device for image: %s\n",
	    cudaGetErrorString(rc));
    return NULL;
  }
  
  return bits;
}

/* Copy the data from host bm_t to device memory.
 * Assumes the buffers are the same size */
int hd_bmcopy( bit *dst, const bm_t *src)
{
  cudaError_t rc = cudaMemcpy( dst, src->bits,
			       sizeof(bit) * src->rows * src->cols,
			       cudaMemcpyHostToDevice );
  
  if (rc != cudaSuccess) 
  {
    fprintf(stderr,"Error copying image to device: %s\n",
	    cudaGetErrorString(rc));
    return -1;
  }
  return 0;
}

/* Copy the data from device memory to host pbm_t.
 * Assumes the buffers are the same size */
int dh_bmcopy( bm_t *dst,  bit *src)
{
  cudaError_t rc = cudaMemcpy( dst->bits, src, 
			       dst->rows * dst->cols * sizeof(bit),
			       cudaMemcpyDeviceToHost );
  
  if (rc != cudaSuccess) {
    fprintf(stderr,"Error copying image to host: %s\n",
	    cudaGetErrorString(rc));
    return -1;
  }
  return 0;
}

/* Copy the data across device memory .
 * Assumes the buffers are the same size */
int dd_bmcopy( bit *dst,  bit *src, int numRows, int numCols)
{
  cudaError_t rc = cudaMemcpy( dst, src,
			       numRows * numCols * sizeof(bit),
			       cudaMemcpyDeviceToDevice );

  if (rc != cudaSuccess) 
  {
    fprintf(stderr,"Error copying image across device: %s\n",
	    cudaGetErrorString(rc));
    return -1;
  }
  return 0;
}