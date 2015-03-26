/* Iterated Conditional Modes (ICM) binary image restoration
 *
 * Created by Jerod Weinman, 9 June 2008
 * Revised 15 August 2012 to include timing
 * Revised 13 August 2014 to use bitmap library and safely verify user input
 */
#include "cubitmap.h"
#include "bitmap.h"
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define DEFAULT_ALPHA 2
#define DEFAULT_BETA 1
#define MAX_ITER 20
#define TILE_WIDTH 4

__device__ int alpha_d;
__device__ int beta_d;
__device__ int rows_d;
__device__ int cols_d;
__device__ int MAX_ITER_D;


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


/*
 * Device update function
 */

__device__ int icmupdate(bit*img_d, bit * restImg_d, bit* workImg_d,int alpha,int beta,
                         int r,int c,int rows,int cols)
{
 int converged = 1; /* Flag indicating whether algorithm has converged */
  float cost[2]; /* cost array for both binary states at a pixel */
  bit *restBits = restImg_d;
  bit *workBits = workImg_d;
  //  int alpha = alpha_d;
  // int beta = beta_d;
 
  int index = r*cols + c;   /* Pixel linear index */

      cost[0] = 0;  /* Initialize costs to zero */
      cost[1] = 0;
      
      
      /* Local cost: for flipping pixel (r,c). This assigns the local
       * cost alpha to the opposite state of that at (r,c) */
      cost[1-img_d[index]] = alpha;
      
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
    
  return converged;
}

/*
 * Kernel performing the icm Updates for each subset of pixels
 * img_d is the original image on the device
 * workImg_d is the work image on the device
 * restImg_d is the rest image on the device
 */

__global__ void icmKernel(bit *img_d, bit* restImg_d, bit *workImg_d, int alpha,int beta, 
                          int rows, int cols)
{

  /* Identify particular threads */
  int r = blockIdx.y * TILE_WIDTH + threadIdx.y; /* Pixel row */
  int c = blockIdx.x * TILE_WIDTH + threadIdx.x; /* Pixel col */


  int converged =0;
 int iter;      /* Iteration loop counter */
 
 for (iter=0 ; iter<MAX_ITER ; iter++)         /* Iterate update/restoration */
  {
    __syncthreads();
    converged = icmupdate (img_d,restImg_d, workImg_d,alpha,beta,r,c,rows,cols);     /* Update */
    __syncthreads();

    if( ( r*cols + c) ==  0)
      {
        printf("%d,%d\n",threadIdx.x,threadIdx.y);

          memcpy(restImg_d,workImg_d,sizeof(bit)* rows * cols);//copy working bits to restbits
      }
    // if (converged)
    //   break;         /* Nothing changed, so we are done and can exit the loop */
   // else          
      //   bmcopy(restImg_h,workImg_h);     /* All pixels updated; copy working buffer */
  }
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
__host__ void runicm(const bm_t *img_h, bm_t *restImg_h, double alpha, double beta)
{
  bm_t *workImg_h; /* An image buffer for storing intermediate results */
  int converged; /* Convergence flag for testing */
   int iter = MAX_ITER;      /* Iteration loop counter */

  int rows = img_h->rows;
  int cols = img_h->cols;
  
 workImg_h  = (bm_t*) malloc (sizeof(bm_t));          /* Allocate bitmap struct */
  
  if (workImg_h==NULL)
  {
    fprintf(stderr,"Unable to allocate work image");
    exit(EXIT_FAILURE);
  }

  if ( bmalloc(workImg_h, img_h->rows, img_h->cols) < 0 )    /* Allocate work image */
    exit(EXIT_FAILURE);
  
  if ( bmcopy(restImg_h, img_h) < 0 )              /* Copy original into restored */
    exit(EXIT_FAILURE);
  

  /* Allocate space on device */
  bit* img_d = cu_bmalloc(rows, cols);
  bit* restImg_d = cu_bmalloc(rows, cols);
  bit* workImg_d = cu_bmalloc(rows, cols);

  /* check if memory allocation was successful*/
  if (img_d==NULL || restImg_d==NULL || workImg_d==NULL) 
  {
    fprintf(stderr,"Error allocating device image memory\n");
    exit(EXIT_FAILURE);
  }

  /* Copy  image data to the device */

  
  if ( hd_bmcopy(img_d, img_h) < 0 ) /* Copy image to device */
  {
    fprintf(stderr,"Error copying original image to device\n");
    exit(EXIT_FAILURE);
  }

  if ( hd_bmcopy(restImg_d, restImg_h) < 0 ) /* Copy image to device */
  {
    fprintf(stderr,"Error copying rest image to device\n");
    exit(EXIT_FAILURE);
  }

  if ( hd_bmcopy(workImg_d, workImg_h) < 0 ) /* Copy image to device */
  {
    fprintf(stderr,"Error copying work image to device\n");
    exit(EXIT_FAILURE);
  }


  /* Allocate global constants on device*/

  cudaError_t rc;

   // Copy value of alpha on host into alpha_d on device
    rc = cudaMemcpyToSymbol( alpha_d, &alpha, sizeof(int), 0, 
                             cudaMemcpyHostToDevice);
 
    if (rc != cudaSuccess)
        fprintf(stderr,"Unable to copy alpha value to device: %s", 
                cudaGetErrorString(rc) );

      // Copy value of alpha on host into alpha_d on device
    rc = cudaMemcpyToSymbol( beta_d, &beta, sizeof(int), 0, 
                             cudaMemcpyHostToDevice);
 
    if (rc != cudaSuccess)
        fprintf(stderr,"Unable to copy beta value to device: %s", 
                cudaGetErrorString(rc) );

        // Copy value of MAX_ITER on host into MAX_ITER_d on device
    rc = cudaMemcpyToSymbol( MAX_ITER_D, &iter, sizeof(int), 0, 
                             cudaMemcpyHostToDevice);
 
    if (rc != cudaSuccess)
        fprintf(stderr,"Unable to copy beta value to device: %s", 
                cudaGetErrorString(rc) );

  /* Set up execution configuration */
  dim3 dimGrid( cols / TILE_WIDTH + 1, rows / TILE_WIDTH + 1 );
  dim3 dimBlock( TILE_WIDTH, TILE_WIDTH );
  
  /* Invoke Kernel*/
  icmKernel<<<dimGrid,dimBlock>>>(img_d,restImg_d,workImg_d,alpha,beta,rows,cols); 


  /*
   * Copy restored image from device to host
   */

  if (dh_bmcopy(restImg_h, restImg_d)<0) /* Copy inverted image to host */
  {
    fprintf(stderr,"Error copying restImg_d  to host");
    exit(EXIT_FAILURE);
  }

  /* free device memory */

  if (cudaFree( img_d ) != cudaSuccess ||   /* Free device memory */
      cudaFree( restImg_d ) != cudaSuccess ||
      cudaFree( workImg_d )  != cudaSuccess ) {
    fprintf(stderr,"Error releasing device memory\n");
  }


  // for (iter=0 ; iter<MAX_ITER ; iter++)         /* Iterate update/restoration */
  // {
  //   converged = icmupdate (img_h, restImg_h, workImg_h, alpha, beta);     /* Update */

  //   if (converged)
  //     break;         /* Nothing changed, so we are done and can exit the loop */
  //   else          
  //     bmcopy(restImg_h,workImg_h);     /* All pixels updated; copy working buffer */
  // }

  bmfree(workImg_h);                    /* Free our temporary work image buffer */
  free(workImg_h);
}

/* Run the ICM algorithm, write the result, and print the run time */
__host__ void run_time_icm(const char* origFile, const char* cleanFile,
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
__host__ float estrtof(char* str, char* cmd, const char* name )
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
__host__ void processArguments(int argc, char* argv[],
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
