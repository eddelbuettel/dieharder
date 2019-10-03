/*
 * $Id: rgb_bitdist.c 225 2006-08-17 13:15:00Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

#include <dieharder/libdieharder.h>

/*
 * A standard test returns a single-pass p-value as an end result.
 * The "size" of the single pass test is controlled by the tsamples
 * parameter, where possible (some tests use statistics evaluated only
 * for very particular values of tsamples.
 *
 * The test is, in general, executed psamples times and the p-values
 * for each run are in turn accumulated in a vector.  This can all be
 * done in a very standard loop.
 *
 * These values SHOULD be uniformly distributed from [0-1].  A
 * Kuiper Kolmogorov-Smirnov test is performed on the final distribution
 * of p to generate a p-value for the entire test series, containing
 * (typically) tsamples*psamples actual samples.
 *
 * Some tests generate more than one p-value per single pass.  Others
 * are designed to be iterated over yet another integer-indexed control
 * parameter.  To facilitate the consistent pass-back of test results to
 * a UI and allow test reuse without leaks, it is important to be able
 * to create "a standard test" and destroy/free it when done.  For this
 * reason the API for the library standard test function is quite
 * object-oriented in its implementation.
 *
 * It is strongly suggested that this object oriented design be reused
 * whenever possible when designing new tests.  This also permits the
 * maximal reuse of code in the UI or elsewhere.
 */

/*
 * Create a new test that will return nkps p-values per single pass,
 * for psamples passes.  dtest is a pointer to a struct containing
 * the test description and default values for tsamples and psamples.
 * This should be called before a test is started in the UI.
 */
Test **create_test(Dtest *dtest, uint tsamples,uint psamples, void (*testfunc)())
{

 int i,j,k;
 Test **newtest;

 /*
  * Here we have to create a vector of tests of length nkps
  */
 newtest = (Test **)malloc((size_t) dtest->nkps*sizeof(Test *));
 for(i=0;i<dtest->nkps;i++){
   newtest[i] = (Test *)malloc(sizeof(Test));
 }

 /*
  * Initialize the newtests
  */
 for(i=0;i<dtest->nkps;i++){
   /*
    * Do a standard test if -a(ll) is selected no matter what people enter
    * for tsamples or psamples.  ALSO use standard values if tsamples or
    * psamples are 0 (not initialized).
    */
   if(all == YES || tsamples == 0){
     newtest[i]->tsamples = dtest->tsamples_std;
   } else {
     newtest[i]->tsamples = tsamples;
   }
   if(all == YES || psamples == 0){
     newtest[i]->psamples = dtest->psamples_std;
   } else {
     newtest[i]->psamples = psamples;
   }
     
   /*
    * Now we can malloc space for the pvalues vector, and a
    * single (80-column) LINE for labels for the pvalues.  We default
    * the label to a line of #'s.
    */
   newtest[i]->pvalues = (double *)malloc((size_t)newtest[i]->psamples*sizeof(double));
   newtest[i]->pvlabel = (char *)malloc((size_t)LINE*sizeof(char));
   snprintf(newtest[i]->pvlabel,LINE,"##################################################################\n");
   for(j=0;j<newtest[i]->psamples;j++){
     newtest[i]->pvalues[j] = 0.0;
   }

   /*
    * This is the actual test function.  It should take a test pointer as
    * input and fill in nkps pvalues at the appropriate offset in the
    * pvalues vector.
    */
   newtest[i]->testfunc = testfunc;

   /*
    * Finally, we initialize ks_pvalue "just because".
    */
   newtest[i]->ks_pvalue = 0.0;

 }

 return(newtest);

}

/*
 * Destroy (free) a test created with create_test without leaking.
 * This should be called as soon as a test is finished in the UI.
 */
void destroy_test(Dtest *dtest, Test **test)
{

 int i;

 /*
  * To destroy a test one has to first free its allocated contents
  * or leak.
  */
 for(i=0;i<dtest->nkps;i++){
   free(test[i]->pvalues);
 }
 for(i=0;i<dtest->nkps;i++){
   free(test[i]);
 }
 free(test);

}

void std_test(Dtest *dtest, Test **test)
{

 int i,j;

 /*
  * Use test[0]->psamples by default.  The others (if any) should
  * be the same anyway, see create_test() above.
  */
 for(i=0;i<test[0]->psamples;i++){

   /*
    * Reseed every sample IF input isn't from a file AND if no seed was
    * specified on the command line.  Note that this should not generally
    * be necessary or desireable, so there really should be an additional
    * control parameter that regulates whether or not this reseeding
    * occurs.  The main reason I do it is so that NO TWO RUNS ARE ALIKE,
    * permitting marginal results to be at least occasionally resolved
    * with a few more runs (although it is better in general to do this
    * by increasing psamples).
    */
   if(fromfile == 0 && Seed == 0){
     seed = random_seed();
     gsl_rng_set(rng,seed);
   }

   test[0]->testfunc(test,i);

 }

 /*
  * evaluate the final test p-values for each individual test statistic
  * computed during the one run of nkps trials.
  */
 for(j = 0;j < dtest->nkps;j++){
   test[j]->ks_pvalue = kstest_kuiper(test[j]->pvalues,test[j]->psamples);
 }

}

