/* bitmap - Utility routines for writing, reading, copying, and
 * allocating portable bimap (PBM) image files
 * 
 * bm_t - Struct defining the portable bitmap image type
 * bmread - Read a pbm file from disk
 * bmwrite - Write a pbm file to disk
 * bmalloc - Allocate memory for the bm_t
 * bmfree - Free a bm_t structure's bits
 *
 * Created by Jerod Weinman, May 2008
 * Revised 13 August 2014
 */
#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "pbmio.h"

typedef struct {
     bit *bits; /* 2D array is a pointer to a ROW-MAJOR order matrix */
     int rows;
     int cols;
} bm_t;

/* Macro for simple linear indexing */
#define SUB2IND(row,col,numcols) (row) * (numcols) + (col) 

/* Read a pbm file from disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in.
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
int bmread( const char *file, bm_t *im);

/* Write a pbm file to disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in.
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
int bmwrite( const char *file,  bm_t *im);

/* Allocate memory for the bm_t  */
int bmalloc( bm_t *im, int rows, int cols);

/* Free a bm structure's bits */
void bmfree( bm_t *im);

/* Copy the data from one bm_t to another.
 * Assumes the buffers are the same size */
int bmcopy( bm_t *dst, const bm_t *src);

/* Convert a bm_t to a pbm_t */
int bm2pbm( const bm_t *src, pbm_t *dst);

/* Convert a pbm_t to a bm_t */
int pbm2bm( const pbm_t *src, bm_t *dst);

#endif
