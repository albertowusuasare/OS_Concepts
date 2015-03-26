/* A parallel program using POSIX threads to invert an image
 *
 * Usage: pinvert input output
 *
 * Created by Jerod Weinman, 12 June 2008
 * Revised 13 August 2014 to use bitmap library
 * 
 */

#include "bitmap.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
  bm_t *img;
  int threadIdx;
  int numThreads;
} invertThr_t;

void* invertThread(void *invertParam);
void threadedInvert(pbm_t *img);

/* Threaded function that performs the inversion on a subset of image rows
 * Inefficiently uses array indexing */
void* invertThread0(void *invertParam)
{
  /* Unpack parameters for easier reference */
  bm_t *img = ((invertThr_t*)invertParam)->img;
  int threadIdx = ((invertThr_t*)invertParam)->threadIdx;
  int numThreads = ((invertThr_t*)invertParam)->numThreads;
  
  /* Do the inversion, but starting at our thread index row, and
   * doing only one every numThread rows. */
  int r,c,i;
  for (r=threadIdx ; r < img->rows ; r+=numThreads)
    for (c=0 ; c < img->cols ; c++)
    {
      i = SUB2IND(r,c,img->cols);  /* Calculate linear index */
      img->bits[i] = !img->bits[i]; /* Invert */
    }

}

/* Threaded function that performs the inversion on a subset of image rows
 * Efficiently uses pointers in loops  */
void* invertThread(void *invertParam)
{
  /* Unpack parameters for easier reference */
  bm_t *img = ((invertThr_t*)invertParam)->img;
  int threadIdx = ((invertThr_t*)invertParam)->threadIdx;
  int numThreads = ((invertThr_t*)invertParam)->numThreads;
  
  int rows = img->rows;
  int cols = img->cols;
  /* Do the inversion, but starting at our thread index row, and
   * doing only one every numThread rows. */
  int r,c;
  bit *pbrow, *pb;

  pbrow = img->bits + threadIdx*cols; /* First pixel of row threadIdx */
  for (r=threadIdx ; r < rows ; r+=numThreads, pbrow += numThreads*cols)
    for (c=0, pb=pbrow ; c < cols ; c++, pb++)
      *pb = !(*pb); /* Invert */
}

/* Set up and run parallel threads to flip every bit in a bitmap image */
void invertImage(bm_t *img, int numThreads)
{
  int res;                        /* Result of starting thread */
  pthread_t threads[numThreads]; /* The threads */
  pthread_attr_t attr;            /* Thread attributes */
  invertThr_t invParams[numThreads]; /* Packaged thread params */
  
  /* Initialize and set thread detached attribute as joinable */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  /* Create threads */  
  int t;
  for (t=0 ; t<numThreads ; t++) 
  {
  /* Package parameters for threads */
    invParams[t].img = img;
    invParams[t].threadIdx = t;
    invParams[t].numThreads = numThreads;
   
    /* Create thread */
    res = pthread_create(&threads[t],NULL,invertThread,(void*)&(invParams[t]));
    
    if (res!=0) 
    {
      perror("Could not create thread\n");
      exit (EXIT_FAILURE);
    }
  }
     
  void* status;
  /* Wait for all threads to complete */
  for (t=0 ; t<numThreads ; t++)
  {
    res = pthread_join(threads[t],&status);
    
    if (res!=0)
    {
      perror("Error joining thread");
      exit (EXIT_FAILURE);
    }
  }
}

int main(int argc, char* argv[])
{
    bm_t im; /* The image */
    int numThreads = 2;

     /* Check arguments */
     if (argc<3 || argc>4) {
         fprintf(stderr,"Usage: %s input output [threads]\n",argv[0]);
         exit (EXIT_FAILURE);
     }
     
     char *endPtr;
     if (argc==4) /* Optional number of threads specified */
     {
       numThreads = (int)strtol(argv[3], &endPtr, 10);
     
       if ((endPtr-argv[3])!=strlen(argv[3]) || numThreads<1)
       {
	 fprintf(stderr,"%s: threads must be a positive integer", argv[0]);
	 exit (EXIT_FAILURE);
       }
     }

     if (bmread(argv[1],&im)<0) /* Read the image */
       exit(EXIT_FAILURE);
     
     invertImage(&im, numThreads); /* Do a multi-threaded inversion */
     
     if (bmwrite(argv[2],&im)<0) /* Store the result */
       exit(EXIT_FAILURE);
}
