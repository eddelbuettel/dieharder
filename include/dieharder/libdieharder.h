#ifndef _LIBDIEHARDER_H_
#define _LIBDIEHARDER_H_

/*
 *========================================================================
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
#include <stdint.h>

/* This turns on uint macro in c99 */
#define __USE_MISC 1
#define _GNU_SOURCE 1
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* This turns on M_PI in math.h */
#define __USE_BSD 1
#ifdef __CYGWIN__
#undef __BSD_VISIBLE
#define __BSD_VISIBLE 1
#endif /* __CYGWIN__ */
#include <math.h>
#ifdef __CYGWIN__
#undef __BSD_VISIBLE
#define __BSD_VISIBLE 0
#endif /* __CYGWIN__ */
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
#include <dieharder/dieharder_rng_types.h>
#include <dieharder/dieharder_test_types.h>

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
 * This is how one gets a macro into quotes; an important one to keep
 * in all program templates.
 */
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)


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
 void dh_header();
 void dh_version();
 double binomial(unsigned int n, unsigned int k, double p);
 double chisq_eval(double *x,double *y,double *sigma, unsigned int n);
 double chisq_poisson(unsigned int *observed,double lambda,int kmax,unsigned int nsamp);
 double chisq_binomial(double *observed,double prob,unsigned int kmax,unsigned int nsamp);
 double chisq_pearson(double *observed,double *expected,int kmax);
 double chisq_uint_uniform_gtest(uint *observed,long numItems,int kmax);
 double sample(void *testfunc());
 double kstest(double *pvalue,int count);
 double kstest_kuiper(double *pvalue,int count);
 double q_ks(double x);
 double q_ks_kuiper(double x,int count);

 void histogram(double *input, char *pvlabel, int inum, double min, double max, int nbins, char *label);

 unsigned int get_bit_ntuple(unsigned int *bitstring,unsigned int bslen,unsigned int blen,unsigned int boffset);
 void dumpbits(unsigned int *data, unsigned int nbits);
 void dumpbitwin(unsigned int data, unsigned int nbits);
 void dumpuintbits(unsigned int *data, unsigned int nbits);
 void cycle(unsigned int *data, unsigned int nbits);
 int get_bit(unsigned int *rand_uint, unsigned int n);
 int get_bit(unsigned int *rand_uint, unsigned int n);
 void dumpbits_left(unsigned int *data, unsigned int nbits);
 unsigned int bit2uint(char *abit,unsigned int blen);
 void fill_uint_buffer(unsigned int *data,unsigned int buflength);
 unsigned int b_umask(unsigned int bstart,unsigned int bstop);
 unsigned int b_window(unsigned int input,unsigned int bstart,unsigned int bstop,unsigned int boffset);
 unsigned int b_rotate_left(unsigned int input,unsigned int shift);
 unsigned int b_rotate_right(unsigned int input, unsigned int shift);
 void get_ntuple_cyclic(unsigned int *input,unsigned int ilen,
    unsigned int *output,unsigned int jlen,unsigned int ntuple,unsigned int offset);
 unsigned int get_uint_rand(gsl_rng *gsl_rng);
 void get_rand_bits(void *result,unsigned int rsize,unsigned int nbits,gsl_rng *gsl_rng);
 void mybitadd(char *dst, int doffset, char *src, int soffset, int slen);
 void get_rand_pattern(void *result,unsigned int rsize,int *pattern,gsl_rng *gsl_rng);
 void reset_bit_buffers();

/* Cruft
 int get_int_bit(unsigned int i, unsigned int n);
*/

 void add_lib_rngs();

 int binary_rank(unsigned int **mtx,int mrows,int ncols);
    
 /*
  *========================================================================
  *                           Global Variables
  *
  * The primary control variables, in alphabetical order, with comments.
  *========================================================================
  */
 extern unsigned int all;              /* Flag to do all tests on selected generator */
 extern unsigned int bits;             /* bitstring size (in bits) */
 extern unsigned int diehard;          /* Diehard test number */
 extern unsigned int generator;        /* GSL generator id number to be tested */
 /*
  * We will still need generator above, if only to select the XOR
  * generator.  I need to make its number something that will pretty much
  * never collide, e.g. -1 cast to a unsigned int.  I'm making an arbitrary
  * decision to set the upper bound of the number of generators that can
  * be XOR'd together to 100, but of course as a macro you can increase or
  * decrease it and recompile.  Ordinarily, users will not select XOR --
  * they will just select multiple generators and XOR will automatically
  * become the generator.
  *
  * Note well that at the moment one will NOT be able to XOR multiple
  * instances of the file or stdin generators, in the latter case for
  * obvious reasons.  One SHOULD be able to XOR a file stream with any
  * of the built in generators.
  */
#define GVECMAX 100
 extern char gnames[GVECMAX][128];  /* VECTOR of names to be XOR'd into a "super" generator */
 extern unsigned int gseeds[GVECMAX];       /* VECTOR of unsigned int seeds used for the "super" generators */
 extern unsigned int gnumbs[GVECMAX];       /* VECTOR of GSL generators to be XOR'd into a "super" generator */
 extern unsigned int gvcount;               /* Number of generators to be XOR'd into a "super" generator */
 extern unsigned int gscount;               /* Number of seeds entered on the CL in XOR mode */
 extern unsigned int help_flag;        /* Help flag */
 extern unsigned int hist_flag;        /* Histogram display flag */
 extern unsigned int iterations;	/* For timing loop, set iterations to be timed */
 extern unsigned int ks_test;          /* Selects the KS test to be used, 0 = Kuiper 1 = Anderson-Darling */
 extern unsigned int list;             /* List all tests flag */
 extern unsigned int List;             /* List all generators flag */
 extern double multiply_p;	/* multiplier for default # of psamples in -a(ll) */
 extern unsigned int ntuple;           /* n-tuple size for n-tuple tests */
 extern unsigned int num_randoms;      /* the number of randoms stored into memory and usable */
 extern unsigned int output_file;      /* equals 1 if you output to file, otherwise 0. */
 extern unsigned int output_format;    /* equals 0 (binary), 1 (unsigned int), 2 (decimal) output */
 extern unsigned int overlap;          /* 1 use overlapping samples, 0 don't (for tests with the option) */
 extern unsigned int psamples;         /* Number of test runs in final KS test */
 extern unsigned int quiet;            /* quiet flag -- surpresses full output report */
 extern unsigned int rgb;              /* rgb test number */
 extern unsigned int sts;              /* sts test number */
 extern unsigned int Seed;             /* user selected seed.  Surpresses reseeding per sample.*/
 extern off_t tsamples;        /* Generally should be "a lot".  off_t is u_int64_t. */
 extern unsigned int user;             /* user defined test number */
 extern unsigned int verbose;          /* Default is not to be verbose. */
 extern double Xweak;          /* "Weak" generator cut-off (one sided) */
 extern double Xfail;          /* "Unambiguous Fail" generator cut-off (one sided) */
 extern unsigned int Xtrategy;         /* Strategy used in TTD mode */
 extern unsigned int Xstep;            /* Number of additional psamples in TTD/RA mode */
 extern unsigned int Xoff;             /* Max number of psamples in TTD/RA mode */
 extern double x_user;         /* Reserved general purpose command line inputs for */
 extern double y_user;         /* use in any new user test. */
 extern double z_user;
  
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
 extern double *ks_pvalue,*ks_pvalue2;
 extern unsigned int kspi;
 extern struct timeval tv_start,tv_stop;
 extern int dummy,idiot;
 extern FILE *fp;
#define MAXFIELDNUMBER 8
 extern char **fields;

 /*
  * Global variables and prototypes associated with file_input and
  * file_input_raw.
  */
 unsigned int file_input_get_rewind_cnt(gsl_rng *rng);
 off_t file_input_get_rtot(gsl_rng *rng);
 void file_input_set_rtot(gsl_rng *rng,unsigned int value);

 extern char filename[K];      /* Input file name */
 extern int fromfile;		/* set true if file is used for rands */
 extern int filenumbits;	/* number of bits per integer */
 /*
  * If we have large files, we can have a lot of rands.  off_t is
  * automagically u_int64_t if FILE_OFFSET_BITS is 64, according to
  * legend.
  */
 extern off_t filecount;	/* number of rands in file */
 extern char filetype;         /* file type */
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
    off_t rptr;
    off_t rtot;
    unsigned int rewind_cnt;
  } file_input_state_t;


 /*
  * rng global vectors and variables for setup and tests.
  */
 extern const gsl_rng_type **types;       /* where all the rng types go */
 extern gsl_rng *rng;               /* global gsl random number generator */

 /*
  * All required for GSL Singular Value Decomposition (to obtain
  * the rank of the random matrix for diehard rank tests).
  */
 extern gsl_matrix *A,*V;
 extern gsl_vector *S,*svdwork;

 extern unsigned long int seed;             /* rng seed of run (?) */
 extern unsigned int random_max;       /* maximum rng returned by generator */
 extern unsigned int rmax;             /* scratch space for random_max manipulation */
 extern unsigned int rmax_bits;        /* Number of valid bits in rng */
 extern unsigned int rmax_mask;        /* Mask for valid section of unsigned int */
 
/*
 * dTuple is used in a couple of my tests, but it seems like an
 * accident waiting to happen with it only 5 dimensions.
 *
 * For the moment we'll restrict ourselves to the five dimensions
 * for which we have Q.  To go further, we need more Q.
 */
#define RGB_MINIMUM_DISTANCE_MAXDIM 5
typedef struct {
  double c[RGB_MINIMUM_DISTANCE_MAXDIM];
} dTuple;
 
#ifdef __CYGWIN__
typedef       unsigned int    uint;
#endif /* __CYGWIN__ */
#endif /* _LIBDIEHARDER_H_ */
