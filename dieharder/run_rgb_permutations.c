/*
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 *  run_rgb_permutations()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_permutations()
{

 /*
  * Declare the results struct.
  */
 Test **rgb_permutations_test;
 uint nt,ntmin,ntmax;

 /*
  * Set any GLOBAL data used by the test.  rgb_ntuple is the value
  * assigned by -n ntuple (max) on the command line, or default 0 which
  * means -- do all ntuples in range 1 to 8.  rgb_permutations_dtest.nkps is
  * the number of pvalues to be returned, which is just ntuple.
  *
  * Note that we have to do this BEFORE creating the test because
  * dtest.nkps depends on the value of ntuple!
  */
 if(ntuple){
   ntmax = ntuple;
   ntmin = ntuple;
 } else {
   /*
    * Default ought to be to test 2 through n permutations, but n > 7 is
    * really quite long.  We quit at n = 7.
    */
   ntmin = 2;
   ntmax = 7;
 }
 if(all == YES) ntmin = 2;

 /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */

 /*
  * We could work on speeding this up.  One round of calls and working
  * down?
  */
 for(nt = ntmin;nt <= ntmax;nt++){
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
   rgb_permutations_test = create_test(&rgb_permutations_dtest,tsamples,psamples,&rgb_permutations);
   /*
    * Set this to pass it into the test and on to report() or table().
    */
   rgb_permutations_test[0]->ntuple = nt;

   /*
    * Set any GLOBAL data used by the test.  Then call the test itself
    * This fills in the results in the Test struct.
    */
   std_test(&rgb_permutations_dtest,rgb_permutations_test);

   /*
    * This is the one true output routine and does it all.  It is basically
    * the one routine that will need to be changed to support GUI instead
    * of CLI operation.
    */
   output(&rgb_permutations_dtest,rgb_permutations_test);

   /*
    * Free any GLOBAL data used by the test.
    */
   destroy_test(&rgb_permutations_dtest,rgb_permutations_test);

 }

}
