/* A parallel program using CUDA to invert a PBM image
 *
 * Created by Jerod Weinman, 2 November 2012
 * Revised 13 August 2014
 */

#include "cubitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define TILE_WIDTH 4

/* Kernel that performs inversion on a subset of pixels 
 * img is the original image on the device 
 */
__global__ void invertKernel(bit *img, int rows, int cols)
{

  int i = blockIdx.y * TILE_WIDTH + threadIdx.y; /* Pixel row */
  int j = blockIdx.x * TILE_WIDTH + threadIdx.x; /* Pixel col */
  int index = i*cols + j;   /* Pixel linear index */
  
  if (i<rows && j<cols)
    img[index] = !img[index];   /* Flip the bit */
}

/* Host function to invert an image */  
__host__ int invert( bm_t *p_orig_h )
{

  int rows = p_orig_h->rows;
  int cols = p_orig_h->cols;

  /* Allocate space on device */
  bit* p_orig_d = cu_bmalloc(rows, cols);
  bit* p_rest_d = cu_bmalloc(rows, cols);
  bit* p_work_d = cu_bmalloc(rows, cols);

  if (p_orig_d==NULL || p_rest_d==NULL || p_work_d==NULL) 
  {
    fprintf(stderr,"Error allocating device image memory\n");
    exit(EXIT_FAILURE);
  }
  
  if ( hd_bmcopy( p_orig_d, p_orig_h) < 0 ) /* Copy image to device */
  {
    fprintf(stderr,"Error copying original image to device\n");
    exit(EXIT_FAILURE);
  }


  /* Set up execution configuration */
  dim3 dimGrid( cols / TILE_WIDTH + 1, rows / TILE_WIDTH + 1 );
  dim3 dimBlock( TILE_WIDTH, TILE_WIDTH );
  
  invertKernel<<<dimGrid,dimBlock>>>(p_orig_d, rows, cols ); /* Invoke */

  if (dh_bmcopy( p_orig_h, p_orig_d)<0) /* Copy inverted image to host */
  {
    fprintf(stderr,"Error copying inverted image to host");
    exit(EXIT_FAILURE);
  }

  if (cudaFree( p_orig_d ) != cudaSuccess ||   /* Free device memory */
      cudaFree( p_rest_d ) != cudaSuccess ||
      cudaFree( p_work_d )  != cudaSuccess ) {
    fprintf(stderr,"Error releasing device memory\n");
  }
  return 0;
}

/* Usage: cuinvert input output */
int main(int argc, char* argv[])
{
  char *origFile, *invertedFile;
  bm_t image;
  int result;
  struct timeval start,end,diff;
  
  /* Process command line arguments */
  if (argc!=3)
  {
    fprintf(stderr,"Usage: %s input output\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  origFile = argv[1];
  invertedFile = argv[2];

  if ( bmread(origFile, &image) < 0 )   /* Read input image */
    exit(EXIT_FAILURE);
  
  result = gettimeofday(&start, NULL);  /* Start timer */
  
  if (result) 
  {
    perror("Could not get start time");
    exit(EXIT_FAILURE);
  }
  
  invert(&image);                      /* Run invert */

  result = gettimeofday(&end, NULL);   /* End timer */
  
  if (result) 
  {
    perror("Could not get end time");
    exit(EXIT_FAILURE);
  }
  
  timersub(&end, &start, &diff);       /* Calculate interval */

  printf("%3u.%06u\n",diff.tv_sec,diff.tv_usec);

  if ( bmwrite(invertedFile, &image) < 0 )
    exit (EXIT_FAILURE);
  
  exit (EXIT_SUCCESS);
}
