/*
 * diehard_rank_32x32 test header.
 */

/*
 * function prototype
 */
void diehard_rank_32x32(Test **test,int irun);

static Dtest diehard_rank_32x32_dtest = {
  "Diehard 32x32 Binary Rank Test",
  "\n\
#==================================================================\n\
#                Diehard 32x32 Binary Rank Test\n\
# This is the BINARY RANK TEST for 31x31 matrices. The leftmost \n\
# 31 bits of 31 random integers from the test sequence are used \n\
# to form a 31x31 binary matrix over the field {0,1}. The rank  \n\
# is determined. That rank can be from 0 to 31, but ranks< 28   \n\
# are rare, and their counts are pooled with those for rank 28. \n\
# Ranks are found for (default) 40,000 such random matrices and\n\
# a chisquare test is performed on counts for ranks 31,30,29 and\n\
# <=28.\n\
#\n\
# As always, the test is repeated and a KS test applied to the\n\
# resulting p-values to verify that they are approximately uniform.\n\
#==================================================================\n",
  100,
  40000,
  1
};

/*
 * Global variables
 */
uint **diehard_rank_32x32_mtx;
