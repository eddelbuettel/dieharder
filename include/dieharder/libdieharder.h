/*
 *========================================================================
 * $Id: libdieharder.h 248 2006-10-09 17:59:54Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "copyright.h"

/* To enable large file support */
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_sf.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_heapsort.h>
#include <gsl/gsl_sort.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <dieharder/Dtest.h>
#include <dieharder/parse.h>
#include <dieharder/verbose.h>
#include <dieharder/Xtest.h>
#include <dieharder/Vtest.h>
#include <dieharder/std_test.h>
#include <dieharder/tests.h>
#include <dieharder/dieharder_types.h>

/*
 *========================================================================
 * Useful defines
 *========================================================================
 */

#define STDIN	stdin
#define STDOUT	stdout
#define STDERR	stderr
#define YES	1
#define NO	0
#define PI      3.141592653589793238462643
#define K       1024
#define LINE    80
#define PAGE    4096
#define M       1048576
#define M_2     2097152
/*
 * For reasons unknown and unknowable, free() doesn't null the pointer
 * it frees (possibly because it is called by value!)  Nor does it return
 * a success value.  In fact, it is just a leak or memory corruption waiting
 * to happen.  Sigh.
 *
 * nullfree frees and sets the pointer it freed back to NULL.  This let's
 * one e.g. if(a) nullfree(a) to safely free a IFF it is actually a pointer,
 * and let's one test on a in other ways to avoid leaking memory.
 */
#define nullfree(a) {free(a);a = 0;}

 /*
  *========================================================================
  * Subroutine Prototypes
  *========================================================================
  */
 unsigned long int random_seed();
 void start_timing();
 void stop_timing();
 double delta_timing();
 void measure_rate();
 void Usage();
 void help();
 double binomial(unsigned int n, unsigned int k, double p);
 double chisq_eval(double *x,double *y,double *sigma, unsigned int n);
 double chisq_poisson(uint *observed,double lambda,int kmax,uint nsamp);
 double chisq_binomial(double *observed,double prob,uint kmax,uint nsamp);
 double sample(void *testfunc());
 double kstest(double *pvalue,int count);
 double kstest_kuiper(double *pvalue,int count);
 double q_ks(double x);
 double q_ks_kuiper(double x);

 uint get_bit_ntuple(uint *bitstring,uint bslen,uint blen,uint boffset);
 void dumpbits(unsigned int *data, unsigned int nbits);
 uint get_uint_rand(gsl_rng *gsl_rng);
 void cycle(unsigned int *data, unsigned int nbits);
 int get_bit(uint *rand_uint, unsigned int n);
 int get_int_bit(uint i, uint n);
 void fill_uint_buffer(uint *data,uint buflength);
 uint b_umask(uint bstart,uint bstop);
 uint b_window(uint input,uint bstart,uint bstop,uint boffset);
 uint b_rotate_left(uint input,uint shift);
 uint b_rotate_right(uint input, uint shift);
 void get_ntuple_cyclic(uint *input,uint ilen,
    uint *output,uint jlen,uint ntuple,uint offset);
 uint get_uint_rand(gsl_rng *gsl_rng);
 void get_rand_bits(void *result,uint rsize,uint nbits,gsl_rng *gsl_rng);
 
 void add_lib_rngs();
    
 /*
  *========================================================================
  *                           Global Variables
  *
  * The primary control variables, in alphabetical order, with comments.
  *========================================================================
  */
 int all;               /* Flag to do all tests on selected generator */
 int binary;            /* Flag to output rands in binary (with -o -f) */
 int bits;              /* bitstring size (in bits) */
 int diehard;           /* Diehard test number */
 int generator;         /* GSL generator id number to be tested */
 int help_flag;         /* Help flag */
 int hist_flag;         /* Histogram display flag */
 int iterations;	/* For timing loop, set iterations to be timed */
 int list;              /* List all tests flag */
 int List;              /* List all generators flag */
 int ntuple;            /* n-tuple size for n-tuple tests */
 int num_randoms;		/* the number of randoms stored into memory and usable */
 int output;		/* equals 1 if you output to file, otherwise 0. */
 int overlap;           /* equals 1 if you really want to use diehard overlap */
 int psamples;          /* Number of test runs in final KS test */
 int quiet;             /* quiet flag -- surpresses full output report */
 int rgb;               /* rgb test number */
 int sts;               /* sts test number */
 uint Seed;             /* user selected seed.  Surpresses reseeding per sample.*/
 off_t tsamples;         /* Generally should be "a lot".  off_t is u_int64_t. */
 int user;              /* user defined test number */
 int verbose;           /* Default is not to be verbose. */
 double x_user;         /* General purpose command line inputs for use */
 double y_user;         /* in any test. */
 double z_user;
  
 /*
  *========================================================================
  *
  * A few more needed variables.
  *
  *      ks_pvalue is a vector of p-values obtained by samples, kspi is
  *      its index.
  *
  *      tv_start and tv_stop are used to record timings.
  *
  *      dummy and idiot are there to fool the compiler into not optimizing
  *      empty loops out of existence so we can time one accurately.
  *
  *      fp is a file pointer for input or output purposes.
  *
  *========================================================================
  */
#define KS_SAMPLES_PER_TEST_MAX 256
 /* We need two of these to do diehard_craps.  Sigh. */
 double *ks_pvalue,*ks_pvalue2;
 unsigned int kspi;
 struct timeval tv_start,tv_stop;
 int dummy,idiot;
 FILE *fp;
#define MAXFIELDNUMBER 8
 char **fields;

 /*
  * Global variables and prototypes associated with file_input and
  * file_input_raw.
  */
 uint file_input_get_rewind_cnt(gsl_rng *rng);
 uint file_input_get_rtot(gsl_rng *rng);
 void file_input_set_rtot(gsl_rng *rng,uint value);

 char filename[K];      /* Input file name */
 int fromfile;		/* set true if file is used for rands */
 int filenumbits;	/* number of bits per integer */
 /*
  * If we have large files, we can have a lot of rands.  off_t is
  * automagically u_int64_t if FILE_OFFSET_BITS is 64, according to
  * legend.
  */
 off_t filecount;	/* number of rands in file */
 char filetype;         /* file type */
/*
 * This struct contains the data maintained on the operation of
 * the file_input rng, and can be accessed via rng->state->whatever
 *
 *  fp is the file pointer
 *  flen is the number of rands in the file (filecount)
 *  rptr is a count of rands returned since last rewind
 *  rtot is a count of rands returned since the file was opened
 *  rewind_cnt is a count of how many times the file was rewound
 *     since its last open.
 */
 typedef struct {
    FILE *fp;
    off_t flen;
    uint rptr;
    uint rtot;
    uint rewind_cnt;
  } file_input_state_t;


 /*
  * rng global vectors and variables for setup and tests.
  */
 const gsl_rng_type **types;       /* where all the rng types go */
 gsl_rng *rng;               /* global gsl random number generator */

 /*
  * All required for GSL Singular Value Decomposition (to obtain
  * the rank of the random matrix for diehard rank tests).
  */
 gsl_matrix *A,*V;
 gsl_vector *S,*svdwork;

 unsigned long int seed;             /* rng seed of run (?) */
 unsigned int random_max;       /* maximum rng returned by generator */
 unsigned int rmax;             /* scratch space for random_max manipulation */
 unsigned int rmax_bits;        /* Number of valid bits in rng */
 unsigned int rmax_mask;        /* Mask for valid section of uint */
 
