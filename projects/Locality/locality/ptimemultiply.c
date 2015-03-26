/* ptimemultiply - Program to multi-threaded multiply two random matrices and
 * time the result over several iterations
 * 
 * Created by Jerod Weinman, 11 August 2014
 * Derived from timemultiply.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <sys/resource.h>
#include <pthread.h>

#include "matrix.h"
#include "matrixop.h"
#include "matrixutil.h"

#define ITERS 3

/* Global variables for  access */
struct matrix_t a,b,c; /* Matrix variables (two factors and a product) */
struct matrix_t a2,b2,c2; 
struct matrix_t *pa,*pb,*pc;
struct matrix_t *pa2,*pb2,*pc2;

/* Parse command line parameters for usage:
 *  ptimemultiply N T where N>0 and T>0 are integers
 *
 * Exits the program with status EXIT_FAILURE if these preconditions
 * are violated, otherwise sets *pMtxSize to N and *pNumThreads to T
 */
void parseParameters(int argc, char* argv[], int* pMtxSz, int* pNumThreads)
{
  if (argc!=3)
  {
    fprintf(stderr,"Usage: %s N T\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  char* endPtr;          /* Temporary parsing check variable */

  /* Parameter parsing */
  *pMtxSz = (int)strtol(argv[1],&endPtr,10); /* Attempt to read size */
  
  if ( (endPtr-argv[1])!=strlen(argv[1]) || /* Failed to parse an integer */
       *pMtxSz<=0 ) /* Must be a positive integer */
  {
    fprintf(stderr,"%s: First parameter N must be a positive integer\n", 
	    argv[0]);
    exit(EXIT_FAILURE);
  }

  *pNumThreads = (int)strtol(argv[2],&endPtr,10); /* Read num threads */
  
  if ( (endPtr-argv[2])!=strlen(argv[2]) || /* Failed to parse an integer */
       *pNumThreads<=0 ) /* Must be a positive integer */
  {
    fprintf(stderr,"%s: Second parameter T must be a positive integer\n", 
	    argv[0]);
    exit(EXIT_FAILURE);
  }

}

/* Allocates fresh matrices and then frees old matrices (if iter!=0);
 *  initializes new matrices */
void updateMatrices(int iter,int mtxSize)
{
  if (iter%2) /* Even iteration? */
  {
    pa = &a;   pb = &b;   pc = &c;
    pa2 = &a2;   pb2 = &b2;   pc2 = &c2;
  } 
  else
  {
    pa = &a2;  pb = &b2;  pc = &c2;
    pa2 = &a;   pb2 = &b;   pc2 = &c;
  }
  
  mtxAlloc(pa, mtxSize, mtxSize);   /* Allocate fresh space for matrices */
  mtxAlloc(pb, mtxSize, mtxSize);   /* This helps make the cache "cold" */
  mtxAlloc(pc, mtxSize, mtxSize);
  
  if (iter)  /* After first iteration, we can free old memory */
  {
    mtxFree(pa2); /* Free old space (to avoid memory leak) */
    mtxFree(pb2); /* after alloc so we don't just get the same stuff back */
    mtxFree(pc2); 
  }
  
  mtxRand(pa);   /* Fill matrices with random values */
  mtxRand(pb);
  mtxRand(pc);
}


/* Repeatedly generate new matrices, time, call multiplier which spawns new
   threads to calculate product, and print wall clock time. */
void timeMultiply(int mtxSize, int numThreads)
{
  int iter; /* Current iteration */
  struct timeval startTime, endTime, diffTime; /* Timer measurements */

  /* Take repeated measurements for greater statistical validity */
  for (iter=0; iter<ITERS ; iter++)
  {
    updateMatrices(iter,mtxSize); /* Alloc/free/rand to cool cache */
    
    if (iter) printf("\t"); /* Print a tab after each value */

    /* Start timer, which includes thread setup/join as overhead */
    if (gettimeofday(&startTime,NULL)<0)
    {
      perror("Unable to start timer");
      exit(EXIT_FAILURE);
    }
	
    parMtxMultiply(pa,pb,pc, numThreads); /* Take the product */

    if (gettimeofday(&endTime,NULL)<0) /* End timer */
    {
      perror("Unable to end timer");
      exit(EXIT_FAILURE);
    }
    timersub(&endTime, &startTime, &diffTime);     /* Measure time interval */
    printf("%3lu.%06lu",diffTime.tv_sec,diffTime.tv_usec); /* Print result */
  }
  printf("\n"); /* Print a friendly newline after the entries */
}
    
    
/* Usage: timemultiply N {min|max} */   
int main(int argc, char* argv[])
{
  int mtxSz;             /* Square variable size */
  int numThreads;        /* Number of threads */

  parseParameters(argc,argv,&mtxSz,&numThreads);
  timeMultiply(mtxSz,numThreads);

  exit(EXIT_SUCCESS);
}
