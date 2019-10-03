/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_diehard_squeeze()
 *========================================================================
 */

#include "dieharder.h"

void run_diehard_squeeze()
{


 /*
  * Declare the results struct.
  */
 Test **diehard_squeeze_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 diehard_squeeze_test = create_test(&diehard_squeeze_dtest,tsamples,psamples,&diehard_squeeze);

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&diehard_squeeze_dtest,diehard_squeeze_test);

 /*
  * Output standard test results.
  */
 output(&diehard_squeeze_dtest,diehard_squeeze_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&diehard_squeeze_dtest,diehard_squeeze_test);

}
