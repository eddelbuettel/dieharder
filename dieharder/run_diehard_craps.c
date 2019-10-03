/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_diehard_craps()
 *========================================================================
 */

#include "dieharder.h"

void run_diehard_craps()
{

 /*
  * Declare the results struct.
  */
 Test **diehard_craps_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 diehard_craps_test = create_test(&diehard_craps_dtest,tsamples,psamples,&diehard_craps);
 diehard_craps_test[0]->ntuple = 0;
 diehard_craps_test[1]->ntuple = 0;

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&diehard_craps_dtest,diehard_craps_test);

 /*
  * Output standard test results.
  */
 output(&diehard_craps_dtest,diehard_craps_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&diehard_craps_dtest,diehard_craps_test);

}
