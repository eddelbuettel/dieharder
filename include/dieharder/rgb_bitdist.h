/*
 * rgb_bitdist test header.
 */

/*
 * function prototype
 */
void rgb_bitdist(Test **test,int irun);

static Dtest rgb_bitdist_dtest = {
  "RGB Bit Distribution Test",
  "rgb_bitdist",
  "\n\
#========================================================================\n\
#                 RGB Bit Distribution Test\n\
# Accumulates the frequencies of all n-tuples of bits in a list\n\
# of random integers and compares the distribution thus generated\n\
# with the theoretical (binomial) histogram, forming chisq and the\n\
# associated p-value.  In this test n-tuples are selected without\n\
# WITHOUT overlap (e.g. 01|10|10|01|11|00|01|10) so the samples\n\
# are independent.  Every other sample is offset modulus of the\n\
# sample index and ntuple_max.\n\
#\n",
  100,     /* Default psamples */
  100000,  /* Default tsamples */
  1        /* We magically make all the bit tests return a single histogram */
};

