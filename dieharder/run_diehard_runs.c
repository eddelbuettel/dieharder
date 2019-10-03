/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_diehard_runs()
 *========================================================================
 */

#include "dieharder.h"

void run_diehard_runs()
{

 /*
  * Declare the results struct.
  */
 Test **diehard_runs_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 diehard_runs_test = create_test(&diehard_runs_dtest,tsamples,psamples,&diehard_runs);
 diehard_runs_test[0]->ntuple = 0;
 diehard_runs_test[1]->ntuple = 0;

 /*
  * Set any GLOBAL data used by the test.
  */
 diehard_runs_rand_uint = (uint *)malloc(diehard_runs_test[0]->tsamples*sizeof(uint));
   
 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&diehard_runs_dtest,diehard_runs_test);

 /*
  * Output standard test results.
  */
 output(&diehard_runs_dtest,diehard_runs_test);

 /*
  * Free any GLOBAL data used by the test.
  */
 free(diehard_runs_rand_uint);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&diehard_runs_dtest,diehard_runs_test);

}
