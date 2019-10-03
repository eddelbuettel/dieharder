/*
 * diehard_sums test header.
 */

/*
 * function prototype
 */
void diehard_sums(Test **test,int irun);

static Dtest diehard_sums_dtest = {
  "Diehard Sums Test",
  "\n\
#==================================================================\n\
#                  Diehard Sums Test\n\
# Integers are floated to get a sequence U(1),U(2),... of uni-  \n\
# form [0,1) variables.  Then overlapping sums,                 \n\
#   S(1)=U(1)+...+U(100), S2=U(2)+...+U(101),... are formed.    \n\
# The S's are virtually normal with a certain covariance mat-   \n\
# rix.  A linear transformation of the S's converts them to a   \n\
# sequence of independent standard normals, which are converted \n\
# to uniform variables for a KSTEST. The  p-values from ten     \n\
# KSTESTs are given still another KSTEST.                       \n\
#\n\
# Note well:  -O causes the old diehard version to be run (more or\n\
# less).  Omitting it causes non-overlapping sums to be used and \n\
# directly tests the overall balance of uniform rands.\n\
#==================================================================\n",
  100,
  100,
  1
};

/*
 * Global memory
 */
double *diehard_sums_rand_dbl;

