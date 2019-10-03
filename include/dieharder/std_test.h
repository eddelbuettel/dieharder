/*
 * This is is the std_test struct.  It is the sole output interface between
 * the user and the test program -- all standard test inputs and returns
 * are passed through this struct.  A test can have additional user-settable
 * arguments, of course, but they must be passed in GLOBAL SHARED VARIABLES
 * defined just for the particular test in question.  This is a bit ugly,
 * but so are void *args argument lists and va_start/va_end for multiple
 * layers of passing arguments.
 */
typedef struct {
  void (*testfunc)();  /* Test function name */
  uint nkps;           /* Number of test statistics created per run */
  uint tsamples;       /* Number of samples per test (if applicable) */
  uint psamples;       /* Number of test runs per final KS p-value */
  double *pvalues;     /* Vector of length psamples to hold test p-values */
  double ks_pvalue;    /* Final KS p-value from run of many tests */
} Test;


Test **create_test(Dtest *dtest, uint tsamples, uint psamples, void (*testfunc)() );
void destroy_test(Dtest *dtest, Test **test);
void std_test(Dtest *dtest, Test **test);

