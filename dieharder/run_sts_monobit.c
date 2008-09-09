/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_sts_monobit()
 *========================================================================
 */

#include "dieharder.h"

void run_sts_monobit()
{


 /*
  * Declare the results struct.
  */
 Test **sts_monobit_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 sts_monobit_test = create_test(&sts_monobit_dtest,tsamples,psamples,&sts_monobit);

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&sts_monobit_dtest,sts_monobit_test);

 /*
  * Output standard test results.
  */
 output(&sts_monobit_dtest,sts_monobit_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&sts_monobit_dtest,sts_monobit_test);

}
