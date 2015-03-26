/* pbmio - Utility routines for writing, reading, copying, and
 * allocating standard portable bimap (PBM) image files in ASCII
 * 
 * pbm_t - Struct defining the portable bitmap image type
 * pbmread - Read a pbm file from disk
 * pbmwrite - Write a pbm file to disk
 * pbmalloc - Allocate memory for the pbm_t
 * pbmfree - Free a pbm structure
 *
 * Created by Jerod Weinman, 9 Jun 2008
 * Revised 13 August 2014
 */
#ifndef __PBMIO_H__
#define __PBMIO_H__

/* Make libnetpbm play nicely with C++, which is necessary to link with CUDA */
extern "C" {
#include <ppm.h>
}

typedef struct {
     bit **bits; /* 2D array is a pointer to a ROW-MAJOR order matrix */
     int rows;
     int cols;
} pbm_t;

/* Read a pbm file from disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in.
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
int pbmread( const char *file, pbm_t *im);

/* Write a pbm file to disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in.
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
int pbmwrite( const char *file,  pbm_t *im);

/* Allocate memory for the pbm_t  */
int pbmalloc( pbm_t *im, int rows, int cols);

/* Free a pbm structure's bits */
void pbmfree( pbm_t *im);

/* Copy the data from one pbm_t to another.
 * Assumes the buffers are the same size */
int pbmcopy( pbm_t *dst, const pbm_t *src);

#endif
