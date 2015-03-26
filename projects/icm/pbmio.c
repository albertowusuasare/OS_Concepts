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
#include "pbmio.h"
#include <stdio.h>
#include <string.h>
#include <pbm.h>
#include <assert.h>

/* Read a pbm file from disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in. 
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
int pbmread( const char* file, pbm_t *im)
{
     FILE *imfp;   /* File stream for image file */
     bit **imbits; /* Pointer to loaded image data */

     assert( im != NULL);
     
     imfp = fopen(file, "r"); /* Open file for reading */

     if (imfp == NULL) {
       perror("Unable to open file");
       return -1;
     }

     imbits = pbm_readpbm(imfp, &im->cols, &im->rows); /* libnetpbm call */
     im->bits = imbits;  /* Set image data */

     fclose(imfp);
}

/* Write a pbm file to disk.
 *
 * If there is any failure, an error is printed to STDERR and a
 * negative value is returned. Otherwise zero is returned and the
 * pbm_t is filled in.
 *
 * Uses libnetpbm function that may cause program abort upon failure.
 */
 int pbmwrite( const char* file, pbm_t *im)
{
     FILE *imfp;  /* File stream for image file */

     assert( im != NULL);
     
     imfp = fopen(file, "w"); /* Open file for reading */

     if (imfp == NULL) {
       perror("Unable to open file");
       return -1;
     }
     
     pbm_writepbm(imfp, im->bits, im->cols, im->rows, 0); /* libnetpbm call */

     return 0;
}


/* Allocate memory for the pbm_t  */
int pbmalloc(pbm_t *im, int rows, int cols)
{
     im->bits = pbm_allocarray(cols, rows);  /* libnetpbm call */

     if (im->bits==NULL) {
       fprintf(stderr,"Unable to allocate space for image.");
       return -1;
     }

     im->rows = rows;
     im->cols = cols;

     return 0;
}

/* Free a pbm structure */
void pbmfree (pbm_t *im)
{
  pbm_freearray(im->bits, im->rows); /* libnetpbm call */
}

/* Copy the data from one pbm_t to another.
 * Assumes the buffers are the same size */
 int pbmcopy( pbm_t *dst, const pbm_t *src)
{
  if (dst->rows != src->rows || dst->cols != src->cols ){
    fprintf(stderr,"Image buffers must be the same size to copy");
    return -1;
  }
     
  int i;
  for (i=0; i < dst->rows ; i++) {
    memcpy(dst->bits[i],src->bits[i], src->cols * sizeof(bit) );
  }
  
  return 0;
}
