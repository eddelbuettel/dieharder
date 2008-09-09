/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_rgb_minimum_distance()
 *========================================================================
 */

#include "dieharder.h"

void run_rgb_minimum_distance()
{

 /*
  * Declare the results struct.
  */
 Test **rgb_minimum_distance_test;
 uint dim,mindim,maxdim;

 /*
  * Set any GLOBAL data used by the test.  rgb_md_dim is the value
  * assigned by -n ntuple on the command line, or default 0 which
  * means -- do all dimension in range 2 to 3.  This also sets
  * rgb_minimum_distance_dtest.nkps to the number of pvalues to be
  * returned, which is just ntuple.
  *
  * Note that we have to do this BEFORE creating the test because
  * dtest.nkps depends on the value of ntuple!
  */
 rgb_mindist_avg = 0.0;
 if(ntuple){
   mindim = ntuple;
   maxdim = ntuple;
 } else {
   /*
    * Default is to test d = 2 and d = 3, at least for the
    * moment.  We'll see from this if we need a vector of
    * test defaults.
    */
   mindim = 2;
   maxdim = 5;
 }

 /*
  * This is again the current assumed default for a standard
  * run of all tests
  */
 if(all == YES){
   mindim = 2;
   maxdim = 5;
 }

 for(dim = mindim;dim<=maxdim;dim++){
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
   rgb_minimum_distance_test = create_test(&rgb_minimum_distance_dtest,tsamples,psamples,&rgb_minimum_distance);
   rgb_minimum_distance_test[0]->ntuple = dim;
   
   /*
    * Set any GLOBAL data used by the test.  Then call the test itself
    * This fills in the results in the Test struct.
    */
   std_test(&rgb_minimum_distance_dtest,rgb_minimum_distance_test);

   /*
    * Display standard test results.
    */
   output(&rgb_minimum_distance_dtest,rgb_minimum_distance_test);

   /*
    * Free any GLOBAL data used by the test.
    */
   destroy_test(&rgb_minimum_distance_dtest,rgb_minimum_distance_test);

 }

}
