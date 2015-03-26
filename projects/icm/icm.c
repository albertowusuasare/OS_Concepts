/* Iterated Conditional Modes (ICM) binary image restoration
 *
 * Created by Jerod Weinman, 9 June 2008
 * Revised 15 August 2012 to include timing
 * Revised 13 August 2014 to use bitmap library and safely verify user input
 */

#include "bitmap.h"
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define DEFAULT_ALPHA 2
#define DEFAULT_BETA 1
#define MAX_ITER 20

/* icmupdate - Calculate one ICM update on an image
 *
 * Produces
 *  converged, an int
 *
 * Preconditions:
 *  img is the original binary image
 *  restImg is the current restored image result
 *  workImg is a buffer for storing the update
 *  img, restImg, and workImg are all the same size
 *
 * Postconditions:
 *  update is stored in workImg
 *  converged==1 indicates update yielded no change from restImg
 */
int icmupdate(const bm_t *img, const bm_t *restImg, bm_t *workImg, 
              float alpha, float beta)
{
  int converged = 1; /* Flag indicating whether algorithm has converged */
  float cost[2]; /* cost array for both binary states at a pixel */
  int r,c;
  int rows = img->rows;  /* Avoid constant struct dereferencing in loops */
  int cols = img->cols;
  bit *restBits = restImg->bits;
  bit *workBits = workImg->bits;  
  int index; /* Linear indices */

  for (r=0 ; r < rows ; r++)
    for (c=0 ; c < cols ; c++)
    {
      cost[0] = 0;  /* Initialize costs to zero */
      cost[1] = 0;
      
      index = SUB2IND(r,c,cols);  /* Calculate linear index */
      
      /* Local cost: for flipping pixel (r,c). This assigns the local
       * cost alpha to the opposite state of that at (r,c) */
      cost[1-img->bits[index]] = alpha;
      
      /* Neighborhood cost: Adds beta to the cost for the opposite
       * value of each neighboring state. Note that the neighboring
       * state values used are from the most recent iteration of the
       * restored image. Also, ordered to maximize TLB hits. */
      if (r>0)
      {
	                cost[1-restBits[index-cols]]   += beta; /* North */
	if (c > 0)      cost[1-restBits[index-cols-1]] += beta; /* NorthWest */
	if (c < cols-1) cost[1-restBits[index-cols+1]] += beta; /* NorthEast */
      }
      if (c > 0)        cost[1-restBits[index-1]]      += beta; /* West */
      if (c < cols-1)   cost[1-restBits[index+1]]      += beta; /* East */
      if (r < rows-1)
      {
	                cost[1-restBits[index+cols]]   += beta;  /* South */
	if (c > 0)      cost[1-restBits[index+cols-1]] += beta; /* SouthWest */
	if (c < cols-1) cost[1-restBits[index+cols+1]] += beta; /* SouthEast */
      }

      /* Assign whichever state has lower cost to the intermediate
       * "working" restored image */
      workBits[index] = (cost[0] > cost[1]);
      
      /* If we still think we are converging, check whether the new
       * value (in workImg) differs from our previous restored
       * value. If they differ, we have not converged */
      if (converged && workBits[index]!=restBits[index])
	converged = 0;
    }
  return converged;
}

/* runicm - Run the ICM algorthm with parameters alpha and beta on an image
 *
 * Preconditions:
 *  img is the original binary image
 *  restImg is an already allocated buffer for the restored image result
 *  img and restImg are the same size
 *
 * Postconditions:
 *  ICM is run on img until convergence or an iteration limit is reached
 *  Result is stored in restImg buffer
 */
void runicm(const bm_t *img, bm_t *restImg, double alpha, double beta)
{
  bm_t *workImg; /* An image buffer for storing intermediate results */
  int converged; /* Convergence flag for testing */
  int iter;      /* Iteration loop counter */
  
  workImg = (bm_t*) malloc (sizeof(bm_t));          /* Allocate bitmap struct */
  
  if (workImg==NULL)
  {
    fprintf(stderr,"Unable to allocate work image");
    exit(EXIT_FAILURE);
  }

  if ( bmalloc(workImg, img->rows, img->cols) < 0 )    /* Allocate work image */
    exit(EXIT_FAILURE);
  
  if ( bmcopy(restImg, img) < 0 )              /* Copy original into restored */
    exit(EXIT_FAILURE);
  
  for (iter=0 ; iter<MAX_ITER ; iter++)         /* Iterate update/restoration */
  {
    converged = icmupdate (img, restImg, workImg, alpha, beta);     /* Update */

    if (converged)
      break;         /* Nothing changed, so we are done and can exit the loop */
    else          
      bmcopy(restImg,workImg);     /* All pixels updated; copy working buffer */
  }
  bmfree(workImg);                    /* Free our temporary work image buffer */
  free(workImg);
}

/* Run the ICM algorithm, write the result, and print the run time */
void run_time_icm(const char* origFile, const char* cleanFile,
		  double alpha, double beta)
{
  int result;                     /* Return value of gettimeofday for testing */
  struct timeval start,end, diff;          /* Clock and interval measurements */
  bm_t origImg, cleanImg;                          /* Input and output images */
  
  if (bmread(origFile, &origImg)<0)                       /* Read input image */
    exit (EXIT_FAILURE);
  
  if (bmalloc(&cleanImg, origImg.rows, origImg.cols) < 0) /* Allocate cleaned */
    exit (EXIT_FAILURE);

  if ( gettimeofday(&start, NULL) )                         /* Get start time */
  { 
    perror("Could not get start time");
    exit (EXIT_FAILURE);
  }

  runicm(&origImg, &cleanImg, alpha, beta);      /* Run restoration algorithm */

  if( gettimeofday(&end, NULL) )                              /* Get end time */
  {
    perror("Could not get end time");
    exit (EXIT_FAILURE);
  }
    
  timersub(&end, &start, &diff);                   /* Calculate time interval */
  printf("%u.%06u\n",diff.tv_sec,diff.tv_usec);           /* Print time (sec) */

  if (bmwrite(cleanFile, &cleanImg)<0)                /* Write restored image */
    exit (EXIT_FAILURE);
  
  bmfree(&cleanImg);                              /* Free our allocated image */
}

/* String to float conversion.
 * Preconditions: 
 *   All of str is the float
 *   str is a null-terminated character array
 *   cmd is a null-terminated character array
 * Postconditions: Prints an error message of the form 
 *   "cmd: name str must be a number" and exits the program with a failure when 
 *    the first precondition is violated. Otherwise returns the parsed number. */
float estrtof(char* str, char* cmd, const char* name )
{
  char* endPtr;             /* First unparseable character (for verification) */
  float num = strtof(str, &endPtr);

  if ( (endPtr-str) != strlen(str) ) /* Verify entire string was parsed */
  {
    fprintf(stderr,"%s: %s %s must be a number\n",cmd,name,str);
    exit (EXIT_FAILURE);
  }

  return num;
}

/* Verify and process command line arguments. 
 * Prints an error message and exits if any prove immediately problematic */
void processArguments(int argc, char* argv[],
		      char** origFile, char** cleanFile,
		      float *alpha, float *beta)
{
  if (argc<3 || argc>5)                     /* Verify optional argument count */
  {
    fprintf(stderr,"Usage: %s input output [alpha] [beta]\n", argv[0]);
    exit (EXIT_FAILURE);
  }
     
  *origFile = argv[1];
  *cleanFile = argv[2];

  if (argc>3)             /* Safely process optional alpha arguments */
    *alpha = estrtof(argv[3],argv[0],"alpha");
  else
    *alpha = DEFAULT_ALPHA;               /* No option given, take the default */
  
  if (argc>4)             /* Safely process optional alpha arguments */
    *beta = estrtof(argv[4],argv[0],"beta");
  else
    *beta = DEFAULT_BETA;                 /* No option given, take the default */
}
/* Main program: Process user input and calculate ICM result 
*  Usage: ./icm input output [alpha] [beta] */
int main(int argc, char* argv[])
{
  char *origFile, *cleanFile;                  /* Input and output file names */
  float alpha,beta;                               /* ICM algorithm parameters */
 
  processArguments(argc,argv,&origFile,&cleanFile,&alpha,&beta);
  run_time_icm(origFile, cleanFile, alpha, beta);    /* Time and write result */
  exit (EXIT_SUCCESS);                                        /* Exit cleanly */
}
