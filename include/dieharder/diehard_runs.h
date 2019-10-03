/*
 * diehard_runs test header.
 */

/*
 * function prototype
 */
void diehard_runs(Test **test,int irun);

static Dtest diehard_runs_dtest = {
  "Diehard Runs Test",
  "\n\
#==================================================================\n\
#                    Diehard Runs Test\n\
#  This is the RUNS test.  It counts runs up, and runs down, \n\
# in a sequence of uniform [0,1) variables, obtained by float-  \n\
# ing the 32-bit integers in the specified file. This example   \n\
# shows how runs are counted:  .123,.357,.789,.425,.224,.416,.95\n\
# contains an up-run of length 3, a down-run of length 2 and an \n\
# up-run of (at least) 2, depending on the next values.  The    \n\
# covariance matrices for the runs-up and runs-down are well    \n\
# known, leading to chisquare tests for quadratic forms in the  \n\
# weak inverses of the covariance matrices.  Runs are counted   \n\
# for sequences of length 10,000.  This is done ten times. Then \n\
# repeated.                                                     \n\
#\n\
# In Dieharder sequences of length tsamples = 100000 are used by\n\
# default, and 100 p-values thus generated are used in a final\n\
# KS test.\n\
#==================================================================\n",
  100,     /* Default psamples */
  100000,  /* Default tsamples */
  2        /* runs returns two pvalues, not just one */
};

/*
 * The following are the definitions and parameters for runs, based on
 * Journal of Applied Statistics v30, Algorithm AS 157, 1981:
 *    The Runs-Up and Runs-Down Tests, by R. G. T. Grafton.
 * (and before that Knuth's The Art of Programming v. 2).
 */

#define RUN_MAX 6

/*
 * a_ij
 */
static double a[6][6] = {
 { 4529.4,   9044.9,  13568.0,   18091.0,   22615.0,   27892.0},
 { 9044.9,  18097.0,  27139.0,   36187.0,   45234.0,   55789.0},
 {13568.0,  27139.0,  40721.0,   54281.0,   67852.0,   83685.0},
 {18091.0,  36187.0,  54281.0,   72414.0,   90470.0,  111580.0},
 {22615.0,  45234.0,  67852.0,   90470.0,  113262.0,  139476.0},
 {27892.0,  55789.0,  83685.0,  111580.0,  139476.0,  172860.0}
};

/*
 * b_i
 */
static double b[6] = {
 1.0/6.0,
 5.0/24.0,
 11.0/120.0,
 19.0/720.0,
 29.0/5040.0,
 1.0/840.0,};

uint *diehard_runs_rand_uint;
