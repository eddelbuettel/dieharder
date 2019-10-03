/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_diehard_rank_6x8()
 *========================================================================
 */

#include "dieharder.h"

void run_diehard_rank_6x8()
{

 int i;

 /*
  * Declare the results struct.
  */
 Test **diehard_rank_6x8_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 diehard_rank_6x8_test = create_test(&diehard_rank_6x8_dtest,tsamples,psamples,&diehard_rank_6x8);

 /*
  * Set any GLOBAL data used by the test.
  */
 diehard_rank_6x8_mtx = (uint **)malloc(6*sizeof(uint *));
 for(i=0;i<6;i++) diehard_rank_6x8_mtx[i] = (uint *)malloc(8*sizeof(uint));

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&diehard_rank_6x8_dtest,diehard_rank_6x8_test);

 /*
  * Output standard test results.
  */
 output(&diehard_rank_6x8_dtest,diehard_rank_6x8_test);

 /*
  * Free global variables allocated above (or leak).
  */
 for(i=0;i<6;i++) free(diehard_rank_6x8_mtx[i]);
 free(diehard_rank_6x8_mtx);
  

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&diehard_rank_6x8_dtest,diehard_rank_6x8_test);

}
