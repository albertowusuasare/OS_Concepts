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
#ifndef __CUBITMAP_H__
#define __CUBITMAP_H__

#include "bitmap.h"

/* Allocate memory for a bitmap on the CUDA device */
bit* cu_bmalloc (int rows, int cols);

/* Copy the data from host bm_t to device memory.
 * Assumes the buffers are the same size */
int hd_bmcopy( bit *dst, const bm_t *src);

/* Copy the data from device memory to host bm_t.
 * Assumes the buffers are the same size */
int dh_bmcopy( bm_t *dst,  bit *src);

/* Copy the data across device memory.
 * Assumes the buffers are the same size */
int dd_bmcopy( bit *dst,  bit *src, int numRows, int numCols);


#endif
