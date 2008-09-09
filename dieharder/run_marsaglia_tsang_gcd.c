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

void run_marsaglia_tsang_gcd()
{


 /*
  * Declare the results struct.
  */
 Test **marsaglia_tsang_gcd_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 marsaglia_tsang_gcd_test = create_test(&marsaglia_tsang_gcd_dtest,tsamples,psamples,&marsaglia_tsang_gcd);
 marsaglia_tsang_gcd_test[0]->ntuple = 0;
 marsaglia_tsang_gcd_test[1]->ntuple = 0;
 
 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&marsaglia_tsang_gcd_dtest,marsaglia_tsang_gcd_test);

 /*
  * Output standard test results.
  */
 output(&marsaglia_tsang_gcd_dtest,marsaglia_tsang_gcd_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&marsaglia_tsang_gcd_dtest,marsaglia_tsang_gcd_test);

}
