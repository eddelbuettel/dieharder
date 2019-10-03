/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_rgb_timing()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_persist()
{

 /*
  * Declare the results struct.
  */
 Rgb_Persist persist;
 Test **rgb_persist_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 rgb_persist_test = create_test(&rgb_persist_dtest,tsamples,psamples,&rgb_persist);

 /*
  * Set any GLOBAL data used by the test.
  *
  * We arbitrarily choose 256 successive random numbers as enough.
  * This should be plenty -- the probability of any bit slot not
  * changing by CHANCE in 256 tries is 2^-256 or almost 10^-26,
  * so even with 32 bit slots, repeatedly, we aren't horribly likely
  * to see it in our lifetime unless we run this test continuously for
  * months at a time (yes, a dumb idea).
  */
 rgb_persist_rand_uint = (unsigned int*)malloc(256 * sizeof(unsigned int));

 /*
  * Show the standard test header for this test.
  */
 show_test_header(&rgb_persist_dtest,rgb_persist_test);

 /*
  * Call the actual test that fills in the results struct.
  */
 rgb_persist(rgb_persist_test,&persist);

 if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
   printf("# %u rands were used in this test\n",file_input_get_rtot(rng));
   printf("# The file %s was rewound %u times\n",gsl_rng_name(rng),file_input_get_rewind_cnt(rng));
 }
 printf("#==================================================================\n");
 printf("#                          Results\n");
 printf("# Results for %s rng, using its %d valid bits:\n",gsl_rng_name(rng),rmax_bits);
 printf("# (Cumulated mask of zero is good.)\n");
 printf("# cumulated_mask = %10u = ",persist.cumulative_mask);
 dumpbits(&persist.cumulative_mask,persist.nbits);
 printf("\n# randm_mask     = %10u = ",rmax_mask);
 dumpbits(&rmax_mask,persist.nbits);
 printf("\n# random_max     = %10u = ",random_max);
 dumpbits(&random_max,persist.nbits);
 if(persist.cumulative_mask){
   printf("\n# rgb_persist test FAILED (bits repeat)\n");
 } else {
   printf("\n# rgb_persist test PASSED (no bits repeat)\n");
 }
 printf("#==================================================================\n");

 free(rgb_persist_rand_uint);

}
