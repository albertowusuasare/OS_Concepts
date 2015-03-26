/* A program to invert a PBM image
 * Usage: invert input output
 *
 * Created by Jerod Weinman, 12 June 2008
 * Revised 13 August 2014
 */
#include "bitmap.h"

/* invertImage -- Flips every bit in an image.
 * "Inefficiently" uses array indexing with 2D loops*/
void invertImage0(bm_t *im)
{
  int r,c,i;
  for (r=0 ; r < im->rows ; r++)
    for (c=0 ; c < im->cols ; c++)
    {
      i = SUB2IND(r,c,im->cols);  /* Calculate linear index */
      im->bits[i] = !im->bits[i]; /* Invert */
    }
}

/* invertImage -- Flips every bit in an image.
 * More efficiently uses array indexing with 1D loop */
void invertImage1(bm_t *im)
{
  int i;
  int len = im->rows * im->cols;
  for (i=0 ; i < len ; i++)
    im->bits[i] = !im->bits[i]; /* Invert */
}

/* invertImage -- Flips every bit in an image.
 * Efficiently uses pointers in loop */
void invertImage(bm_t *im)
{
  int i;
  int len = im->rows * im->cols;
  bit *pb = im->bits;
  for (i=0 ; i < len ; i++, pb++)
    *pb = !(*pb); /* Invert */
}

/* Usage: invert input output */
int main(int argc, char* argv[])
{
  bm_t im; /* The image */
  
  /* Check arguments */
  if (argc!=3) 
  {
    fprintf(stderr,"Usage: %s input output",argv[0]);
    exit (EXIT_FAILURE);
  }

  if (bmread(argv[1],&im)<0) /* Read the image */
    exit(EXIT_FAILURE);

  invertImage(&im); /* Do the inversion */

  if (bmwrite(argv[2],&im)<0) /* Store the result */
    exit(EXIT_FAILURE);

  exit(EXIT_SUCCESS);
}
