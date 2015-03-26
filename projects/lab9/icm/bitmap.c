/* bitmap - Wrapper utility routines for writing, reading, copying, and
 * allocating standard portable bimap (PBM) image files in ASCII
 * 
 * bm_t - Struct defining the bitmap image type
 * bmread - Read a pbm file from disk
 * bmwrite - Write a pbm file to disk
 * bmalloc - Allocate memory for a bitmap
 * bmfree - Free a bm structure
 *
 * Created by Jerod Weinman, 13 August 2014
 */
#include "bitmap.h"
#include <stdio.h>
#include <string.h>
#include <pbm.h>
#include <assert.h>

/* Read a pbm file from disk. */
int bmread( const char* file, bm_t *bm)
{
  pbm_t pbm;
  int rc;

  rc = pbmread(file, &pbm);

  if (rc<0)
    return rc;
  
  rc = pbm2bm(&pbm,bm);
  pbmfree(&pbm); /* Unconditionaly free bits alloc'd by pbmread */

  return rc;
}

/* Write a pbm file to disk. */
int bmwrite( const char* file, bm_t *bm)
{
  pbm_t pbm;
  int rc;

  rc = bm2pbm(bm, &pbm);
  
  if (rc<0)
    return rc;

  rc = pbmwrite(file, &pbm);
  pbmfree(&pbm); /* Unconditionaly free bits alloc'd by bm2pbm */

  return rc;
}


/* Allocate memory for the bm_t  */
 int bmalloc(bm_t *bm, int rows, int cols)
{
  bm->bits = (bit*)malloc (sizeof(bit) * cols * rows); /* Allocate 1d array */
	
  if (NULL==bm->bits)
  {
    perror("Error allocating bits for bitmap");
    return -1;
  }

  bm->rows = rows;
  bm->cols = cols;
  return 0;
}

/* Free a bm structure */
void bmfree (bm_t *bm) { free(bm->bits); }

/* Copy the data from one pbm_t to another.
 * Assumes the buffers are the same size */
 int bmcopy( bm_t *dst, const bm_t *src)
{
  if (dst->rows != src->rows || dst->cols != src->cols )
  {
    fprintf(stderr,"Unable to copy: bitmaps' sizes differ");
    return -1;
  }
     
  memcpy(dst->bits,src->bits, sizeof(bit) * src->rows * src->cols );
  
  return 0;
}

/* Convert a pbm_t to a bm_t */
int pbm2bm( const pbm_t *src, bm_t *dst)
{
  int rc = bmalloc(dst, src->rows, src->cols);

  if (rc<0)
    return rc;

  /* Copy 2d bits to flat 1d array */
  int r; 
  int len = sizeof(bit)*src->cols;
  bit *bitrow = dst->bits;
  
  for (r=0 ; r<dst->rows ; r++, bitrow+=dst->cols)
    memcpy(bitrow, src->bits[r], len);

  return 0;
}

/* Convert a bm_t to a pbm_t */
int bm2pbm( const bm_t *src, pbm_t *dst)
{
  int rc = pbmalloc(dst, src->rows, src->cols);
	
  if (rc<0)
  {
    perror("Error allocating pbm");
    return -1;
  }

  /* Copy flat 1d array to 2d bits */
  int r; 
  int len = sizeof(bit)*src->cols;
  bit *bitrow = src->bits;
  for (r=0 ; r < src->rows ; r++, bitrow+=dst->cols)
    memcpy(dst->bits[r], bitrow, len);

  return 0;
}
