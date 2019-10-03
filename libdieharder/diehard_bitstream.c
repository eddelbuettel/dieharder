/*
 * $Id: diehard_bitstream.c 231 2006-08-22 16:18:05Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard BITSTREAM test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 *                   THE BITSTREAM TEST                          ::
 * The file under test is viewed as a stream of bits. Call them  ::
 * b1,b2,... .  Consider an alphabet with two "letters", 0 and 1 ::
 * and think of the stream of bits as a succession of 20-letter  ::
 * "words", overlapping.  Thus the first word is b1b2...b20, the ::
 * second is b2b3...b21, and so on.  The bitstream test counts   ::
 * the number of missing 20-letter (20-bit) words in a string of ::
 * 2^21 overlapping 20-letter words.  There are 2^20 possible 20 ::
 * letter words.  For a truly random string of 2^21+19 bits, the ::
 * number of missing words j should be (very close to) normally  ::
 * distributed with mean 141,909 and sigma 428.  Thus            ::
 *  (j-141909)/428 should be a standard normal variate (z score) ::
 * that leads to a uniform [0,1) p value.  The test is repeated  ::
 * twenty times.                                                 ::
 *
 *                         Comment
 * Damn good test.  Very few generators in the GSL survive it,
 *
 * The tests BITSTREAM, OPSO, OQSO and DNA are all closely related.
 * They all measure global distribution properties of extended
 * bit combinations in projections of varying dimension.  I
 * believe that they are closely related to the Knuth tests that
 * will code when diehard is finished that look for N-dimensional
 * hyperplanes (known to be a problem with pretty much all
 * linear congruential generators, for example).  I would LIKE to
 * have a series of tests like this that is more systematic and
 * not quite so creatively named to emphasize the relationship
 * between the tests, and I suspect that either Knuth or a homebrew
 * RGB test (like bitdist, but looking at projections of different
 * dimension) would be the way to go here.  Note that this test is
 * also closely related to rgb_bitdist, which measures (as much as
 * possible) the uniformity of the distribution of ntuples of bits.
 *
 * However, ALL rng's fail rgb_bitdist long before they get to
 * 20 bit strings -- the distributions of ntuples stop being random
 * (uniform to the extent required by true randomness) by the time you
 * hit six bit ntuples even with a good rng.  This is an area of
 * future research, as a "proper" test procedure -- in my opinion --
 * would a) determine test dependencies -- if test X is failed, then
 * test Y will always be failed, for example (more generally, the
 * covariance of test failure); b) determine a SYSTEMATIC series of
 * tests that make specific, useful statements about where and how
 * a failure occurs -- basically, tests that determine the MOMENT of
 * the failures in a generalized sense in a suitable series.
 *
 * There may well yet remain specific "oddball" tests that are failed
 * even when moment tests are passed -- in principle, of course, one
 * could test EVERY known distribution that can be generated from
 * uniform deviates and measure the extent to which the generated
 * distribution differs from the ideal -- but I suspect that functional
 * analysis applied to the underlying distributions ultimately connects
 * most failures to an observed failure pattern in a moment study via
 * projective covariance in functional expansion space.
 *========================================================================
 */


#include <dieharder/libdieharder.h>

void diehard_bitstream(Test **test, int irun)
{

 uint i,j,t,boffset;
 Xtest ptest;
 char *w;

 /*
  * p = 141909, with sigma 428, for test[0]->tsamples = 2^21 20 bit ntuples.
  * a.k.a. the number of 20 bit integers missing from 2^21 random
  * samples drawn from this field.  At some point, I should be able
  * to figure out the expected value for missing integers as a rigorous
  * function of the size of the field sampled and number of samples drawn
  * and hence make this test capable of being run with variable sample
  * sizes, but at the moment I cannot do this and so test[0]->tsamples cannot be
  * varied.  Hence we work with diehard's values and hope that they are
  * correct.
  *
  * ptest.x = number of "missing ntuples" given 2^21 trials
  * ptest.y = 141909
  * ptest.sigma = 428
  */
 ptest.y = 141909.0;
 ptest.sigma = 428.0;

 /*
  * We now make test[0]->tsamples measurements, as usual, to generate the
  * missing statistic.  The easiest way to proceed is to just increment
  * a vector of length 2^20 using the generated ntuples as the indices
  * of the slot being incremented.  Then we zip through the vector
  * counting the remaining zeros.
  *
  * The validity of this test SHOULDN'T depend strongly on whether or
  * not the bit strings sampled are "overlapping" (something Marsaglia
  * did frequently I think because his supply of random numbers was so
  * limited compared to the size of the spaces he was sampling).  I
  * therefore implemented it without overlap -- every 20 bit string sampled
  * is truly independent of the rest.  Overlap DOES make me nervous, as
  * it is effectively a left/right shift operation on the bit string plus
  * the addition of a single random bit.  That is, it samples points as
  * 2*previous_point + random_bit, in principle throughout the entire
  * string.  This makes me doubt that the "independent samples" requirement for
  * sampling a distribution has been satisfied by Marsaglia's implementation,
  * so that the sample size is actually smaller than he believes it to be.
  * To put it another way, although the generator itself may well be producing
  * random bitstrings, the overlapping bitstrings sampled by the test
  * obviously have significant bit-level correlations by construction!
  *
  * In any event, an interesting consequence of implementing it with
  * completely disjoint 20 bit integers drawn from the GSL generators is
  * that NONE OF THEM PASS THIS TEST any more, at least on 100 trials.
  * I am very, very tempted to see if re-implementing it (with a #define
  * flag I can toggle at compile time so as not to lose the current
  * implementation) as OVERLAPPING causes the "good" GSL generators to
  * pass all of a sudden.  That would be worth publishing right there.
  */

 w = (char *)malloc(M*sizeof(char));
 memset(w,0,M*sizeof(char));

/*
 printf("w is allocated and zero'd\n");
 printf("About to generate %u samples\n",test[0]->tsamples);
 */

 /*
  * To minimize the number of rng calls, we use each j and k mod 32
  * to determine the offset of the 10-bit long string (with
  * periodic wraparound) to be used for the next iteration.  We
  * therefore have to "seed" the process with a random k.
  */
 i = gsl_rng_get(rng);
 for(t=0;t<test[0]->tsamples;t++){
   if(overlap){
     /*
      * Let's do this the cheap/easy way first, sliding a 20 bit
      * window along each int for the 32 possible starting
      * positions a la birthdays, before trying to slide it all
      * the way down the whole random bitstring implicit in a
      * long sequence of random ints.  That way we can exit
      * the test[0]->tsamples loop at test[0]->tsamples = 2^15...
      */
     if(test[0]->tsamples%32 == 0) {
       i = gsl_rng_get(rng);
       boffset = 0;
     }
     j = get_bit_ntuple(&i,1,20,boffset);
     w[j]++;
     boffset++;
   } else {
     /*
      * Get a 20-bit ntuple as an index into w.  Use each (presumed
      * random) value to determine the uint offset for the next
      * 20-bit window.
      */
     boffset = i%32;
     i = gsl_rng_get(rng);
     i = get_bit_ntuple(&i,1,20,boffset);
     w[i]++;
   }
 }

 /*
  * Now we count the holes, so to speak
  */
 ptest.x = 0;
 for(i=0;i<M;i++){
   if(w[i] == 0){
     ptest.x++;
     /* printf("ptest.x = %f  Hole: w[%u] = %u\n",ptest.x,i,w[i]); */
   }
 }
 if(verbose == D_DIEHARD_BITSTREAM || verbose == D_ALL){
   printf("%f %f %f\n",ptest.y,ptest.x,ptest.x-ptest.y);
 }

 Xtest_eval(&ptest);
 test[0]->pvalues[irun] = ptest.pvalue;

 MYDEBUG(D_DIEHARD_BITSTREAM) {
   printf("# diehard_bitstream(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 /*
  * Don't forget to free or we'll leak.  Hate to have to wear
  * depends...
  */
 nullfree(w);

}

