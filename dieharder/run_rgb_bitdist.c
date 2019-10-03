/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_rgb_bitdist()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_bitdist()
{

 /*
  * Declare the results struct.
  */
 Test **rgb_bitdist_test;
 uint nt,ntmin,ntmax;

 /*
  * Set any GLOBAL data used by the test.  rgb_ntuple is the value
  * assigned by -n ntuple (max) on the command line, or default 0 which
  * means -- do all ntuples in range 1 to 8.  rgb_bitdist_dtest.nkps is
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
    * Default is to test 1 through 12 bits, which takes
    * around 30 minutes on my laptop but is a VERY thorough
    * test of randomness.
    */
   ntmin = 1;
   ntmax = 12;
 }
 if(all == YES) ntmin = 1;

 /* printf("Setting ntmin = %d ntmax = %d\n",ntmin,ntmax); */
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
   rgb_bitdist_test = create_test(&rgb_bitdist_dtest,tsamples,psamples,&rgb_bitdist);
   /*
    * Set this to pass it into the test and on to report() or table().
    */
   rgb_bitdist_test[0]->ntuple = nt;

   /*
    * Set any GLOBAL data used by the test.  Then call the test itself
    * This fills in the results in the Test struct.
    */
   std_test(&rgb_bitdist_dtest,rgb_bitdist_test);

   /*
    * This is the one true output routine and does it all.  It is basically
    * the one routine that will need to be changed to support GUI instead
    * of CLI operation.
    */
   output(&rgb_bitdist_dtest,rgb_bitdist_test);

   /*
    * Free any GLOBAL data used by the test.
    */
   destroy_test(&rgb_bitdist_dtest,rgb_bitdist_test);

 }

}
