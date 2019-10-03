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

void run_rgb_timing()
{

 /*
  * Declare the results struct.
  */
 Rgb_Timing timing;
 Test **rgb_timing_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 rgb_timing_test = create_test(&rgb_timing_dtest,tsamples,psamples,&rgb_timing);

 /*
  * Set any GLOBAL data used by the test.
  */

 /*
  * Show the standard test header for this test.
  */
 show_test_header(&rgb_timing_dtest,rgb_timing_test);

 /*
  * Call the actual test that fills in the results struct.
  */
 rgb_timing(rgb_timing_test,&timing);

 /*
  * Display the results.
  */
 printf("#========================================================================\n");
 printf("# rgb_timing() test using the %s generator \n",gsl_rng_name(rng));
 printf("# Average time per rand = %e nsec.\n",timing.avg_time_nsec);
 printf("# Rands per second = %e.\n",timing.rands_per_sec);

}
