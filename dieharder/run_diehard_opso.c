/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_diehard_opso()
 *========================================================================
 */

#include "dieharder.h"

void run_diehard_opso()
{


 /*
  * Declare the results struct.
  */
 Test **diehard_opso_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 diehard_opso_test = create_test(&diehard_opso_dtest,tsamples,psamples,&diehard_opso);

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&diehard_opso_dtest,diehard_opso_test);

 /*
  * Output standard test results.
  */
 output(&diehard_opso_dtest,diehard_opso_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&diehard_opso_dtest,diehard_opso_test);

}
