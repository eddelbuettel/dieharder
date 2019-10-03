/*
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 *  run_rgb_permutations()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_lagged_sums()
{

 /*
  * Declare the results struct.
  */
 Test **rgb_lagged_sums_test;
 uint lag,lagmin,lagmax;

 /*
  * Set any GLOBAL data used by the test.  rgb_ntuple is the value
  * assigned by -n ntuple (max) on the command line, or default 0 which
  * means -- do all ntuples in range 0 to 32.
  */
 if(ntuple){
   lagmax = ntuple;
   lagmin = ntuple;
 } else {
   /*
    * Test out to quite long lags -- adjusted so it finishes in a
    * "reasonable" amount of time.
    */
   lagmin = 0;
   lagmax = 32;
 }
 /*
  * We could work on speeding this up.  One round of calls and working
  * down?
  */
 for(lag = lagmin;lag <= lagmax;lag++){
   /*
    * We want to use Seed for every loop through if this is a
    * test run with fixed seed at all.
    */
   if(Seed){
     gsl_rng_set(rng,Seed);
   }

   /*
    * First we create the test (to set some values displayed in test header
    * correctly).
    */
   rgb_lagged_sums_test = create_test(&rgb_lagged_sums_dtest,tsamples,psamples,&rgb_lagged_sums);
   /*
    * Set this to pass it into the test and on to report() or table().
    */
   rgb_lagged_sums_test[0]->ntuple = lag;

   /*
    * Set any GLOBAL data used by the test.  Then call the test itself
    * This fills in the results in the Test struct.
    */
   std_test(&rgb_lagged_sums_dtest,rgb_lagged_sums_test);

   /*
    * This is the one true output routine and does it all.  It is basically
    * the one routine that will need to be changed to support GUI instead
    * of CLI operation.
    */
   output(&rgb_lagged_sums_dtest,rgb_lagged_sums_test);

   /*
    * Free any GLOBAL data used by the test.
    */
   destroy_test(&rgb_lagged_sums_dtest,rgb_lagged_sums_test);

 }

}
