/* timemultiply - Program to multiply two random matrices and
 * time the result over several iterations
 * 
 * Created by Jerod Weinman, 8 August 2014
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <sys/resource.h>
#include <pthread.h>
#include <sched.h>

#include "matrix.h"
#include "matrixop.h"
#include "matrixutil.h"

#define ITERS 3

/* Global variables for child thread access */
struct matrix_t a,b,c; /* Matrix variables (two factors and a product) */
struct matrix_t a2,b2,c2; 
struct matrix_t *pa,*pb,*pc;
struct matrix_t *pa2,*pb2,*pc2;

/* Pin caller to a single CPU core
 *
 * Preconditions: 
 *   mask points to a valid cpu_set_t
 * Postconditions:
 *   mask contains only a single CPU core
 * Process:
 *   Uses _GNU_SOURCE macros CPU_ZERO and CPU_SET
 */
void setCPU(cpu_set_t *mask)
{
  CPU_ZERO(mask); /* Clear mask */
  CPU_SET( 1, mask); /* Add CPU zero to the CPU set */
}

/* Parse command line parameters for usage:
 *  timemultiply N {min|max} where N>0 is an integer
 *
 * Exits the program with status EXIT_FAILURE if these preconditions
 * are violated, otherwise sets *pMtxSize to N and *pDoMin to 1 if the
 * method is min (zero otherwise)
 */
void parseParameters(int argc, char* argv[], 
		     int* pMtxSz, int* pDoMin)
{
  if (argc!=3)
  {
    fprintf(stderr,"Usage: %s N {min|max}\n",argv[0]);
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

  /* Verify second parameter as min or max */
  if (strcmp(argv[2],"min") && strlen(argv[2])==3 ) 
    *pDoMin = 1;
  else if (strcmp(argv[2],"max") && strlen(argv[2])==3)
    *pDoMin = 0;
  else
  {
    fprintf(stderr,"%s: Second parameter must be either min or max\n",argv[0]);
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
  
  mtxRand(pa);   /* Fill with random values */
  mtxRand(pb);
  mtxRand(pc);
}

/* Thread routine: Pins thread to CPU, performs the appropriate
 * multiplication algorithm, times the result, totals CPU+system time,
 * prints the time and returns */
void* multiplyTime(void* arg)
{
  int doMin = (int)arg;    /* Cast argument, indicating multiplication alg. */
  struct rusage usage;     /* Thread resources */
  struct timeval ru_ttime; /* Total time CPU (user+sys) used */
  cpu_set_t cpu_mask;      /* Set of CPUs to pin calculations to */

  /* Pin to single processor */
  setCPU(&cpu_mask);
  if (sched_setaffinity(0, sizeof(cpu_set_t), &cpu_mask))
  {
    perror("Error pinning thread");
    exit(EXIT_FAILURE); /* Dramatically kill entire process */
  }

  if (doMin)               /* Do the appropriate multiplication */
    mtxMultiplyMin(pa,pb,pc);
  else
    mtxMultiplyMax(pa,pb,pc);
  /* Note: RUSAGE_THREAD Requires compiling with _GNU_SOURCE */
  int rc = getrusage(RUSAGE_THREAD, &usage);

  if (rc)
  {
    perror("Error retrieving usage statistics");
    exit(EXIT_FAILURE); /* Dramatically kill entire process */
  }
  /* Add user+sys time */
  timeradd(&(usage.ru_utime),&(usage.ru_stime),&ru_ttime); 
  printf("%3lu.%06lu",ru_ttime.tv_sec,ru_ttime.tv_usec); /* Print the result */
  pthread_exit(NULL); /* Explicitly exit the thread */
}

/* Repeatedly generate new matrices, spawn new threads to calculate
   product, time, and print total user+system=cpu time. */
void timeMultiply(int mtxSize, int doMin)
{
  int iter;
  pthread_t thread;
  pthread_attr_t attr;
  void *status;
  int rc;

  /* Initialize and set thread as joinable so we can wait on it */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
  /* Take repeated measurements for greater statistical validity */
  for (iter=0; iter<ITERS ; iter++)
  {
    updateMatrices(iter,mtxSize); /* Alloc/free/rand to cool cache */
    
    if (iter) printf("\t"); /* Print a tab after each value */

    /* Create thread to run computation */
    rc = pthread_create(&thread, NULL, multiplyTime, (void*)doMin);

    if (rc) /* Check for success */
    {
      perror("Unable to create thread");
      exit(EXIT_FAILURE);
    }
    
    rc = pthread_join(thread, &status); /* Wait for thread to complete */

    if (rc) /* Check for success */
    {
      perror("Unable to join thread");
      exit(EXIT_FAILURE);
    }
  }
  pthread_attr_destroy(&attr); /* Free attribute */
  printf("\n"); /* Print a friendly newline after the entries */
}
    
    
/* Usage: timemultiply N {min|max} */   
int main(int argc, char* argv[])
{
  int mtxSz;             /* Square variable size */
  int doMin;             /* Method indicator variable */

  parseParameters(argc,argv,&mtxSz,&doMin);
  timeMultiply(mtxSz,doMin);

  exit(EXIT_SUCCESS);
}
