/*
 * $Id: diehard_oqso.c 228 2006-08-19 05:20:16Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard QPSO test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 *     OQSO means Overlapping-Quadruples-Sparse-Occupancy        ::
 * The test OQSO is similar, except that it considers 4-letter ::
 * words from an alphabet of 32 letters, each letter determined  ::
 * by a designated string of 5 consecutive bits from the test    ::
 * file, elements of which are assumed 32-bit random integers.   ::
 * The mean number of missing words in a sequence of 2^21 four-  ::
 * letter words,  (2^21+3 "keystrokes"), is again 141909, with   ::
 * sigma = 295.  The mean is based on theory; sigma comes from   ::
 * extensive simulation.                                         ::
 *
 * Note 2^21 = 2097152
 * The tests BITSTREAM, OPSO, OQSO and DNA are all closely related.
 *========================================================================
 */


#include <dieharder/libdieharder.h>

void diehard_oqso(Test **test, int irun)
{

 uint i,j,k,l,i0,j0,k0,l0,t,boffset;
 Xtest ptest;
 char ****w;

 /*
  * p = 141909, with sigma 295, FOR tsamples 2^21 2 letter words.
  * These cannot be varied unless one figures out the actual
  * expected "missing works" count as a function of sample size.  SO:
  *
  * ptest.x = number of "missing words" given 2^21 trials
  * ptest.y = 141909
  * ptest.sigma = 295
  */
 ptest.x = 0.0;  /* Initial value */
 ptest.y = 141909.0;
 ptest.sigma = 295.0;

 /*
  * We now make tsamples measurements, as usual, to generate the
  * missing statistic.  We proceed exactly as we did in opso, but
  * with a 4d 32x32x32x32 matrix and 5 bit indices.  This should
  * basically be strongly related to a Knuth hyperplane test in
  * four dimensions.  Equally obviously there is a sequence of
  * tests, all basically identical, that can be done here much
  * as rgb_bitdist tries to do them.  I'll postpone thinking about
  * this in detail until I'm done with diehard and some more of STS
  * and maybe have implemented the REAL Knuth tests from the Art of
  * Programming.
  */

 w = (char ****)malloc(32*sizeof(char ***));
 for(i=0;i<32;i++){
   w[i] = (char ***)malloc(32*sizeof(char **));
   for(j=0;j<32;j++){
     w[i][j] = (char **)malloc(32*sizeof(char *));
     for(k=0;k<32;k++){
       w[i][j][k] = (char *)malloc(32*sizeof(char));
       /* Zero the column */
       memset(w[i][j][k],0,32*sizeof(char));
     }
   }
 }

 /*
  * To minimize the number of rng calls, we use each j and k mod 32
  * to determine the offset of the 10-bit long string (with
  * periodic wraparound) to be used for the next iteration.  We
  * therefore have to "seed" the process with a random l
  */
 l = gsl_rng_get(rng);
 for(t=0;t<test[0]->tsamples;t++){
   if(overlap){
     /*
      * Let's do this the cheap/easy way first, sliding a 20 bit
      * window along each int for the 32 possible starting
      * positions a la birthdays, before trying to slide it all
      * the way down the whole random bitstring implicit in a
      * long sequence of random ints.  That way we can exit
      * the tsamples loop at tsamples = 2^15...
      */
     if(t%32 == 0) {
       i0 = gsl_rng_get(rng);
       j0 = gsl_rng_get(rng);
       k0 = gsl_rng_get(rng);
       l0 = gsl_rng_get(rng);
       boffset = 0;
     }
     /*
      * Get four "letters" (indices into w)
      */
     i = get_bit_ntuple(&i0,1,5,boffset);
     j = get_bit_ntuple(&j0,1,5,boffset);
     k = get_bit_ntuple(&k0,1,5,boffset);
     l = get_bit_ntuple(&l0,1,5,boffset);
     /* printf("%u:   %u  %u  %u  %u  %u\n",t,i,j,k,l,boffset); */
     w[i][j][k][l]++;
     boffset++;

   } else {
     /*
      * Get four "letters" (indices into w)
      */
     boffset = l%32;
     i = gsl_rng_get(rng);
     i = get_bit_ntuple(&i,1,5,boffset);
     boffset = i%32;
     j = gsl_rng_get(rng);
     j = get_bit_ntuple(&j,1,5,boffset);
     boffset = j%32;
     k = gsl_rng_get(rng);
     k = get_bit_ntuple(&k,1,5,boffset);
     boffset = k%32;
     l = gsl_rng_get(rng);
     l = get_bit_ntuple(&l,1,5,boffset);
     w[i][j][k][l]++;
   }
 }
 /*
  * Now we count the holes, so to speak
  */
 ptest.x = 0.0;
 for(i=0;i<32;i++){
   for(j=0;j<32;j++){
     for(k=0;k<32;k++){
       for(l=0;l<32;l++){
         if(w[i][j][k][l] == 0){
           ptest.x += 1.0;
           /* printf("ptest.x = %f  Hole: w[%u][%u][%u][%u] = %u\n",ptest.x,i,j,k,l,w[i][j][k][l]); */
	 }
       }
     }
   }
 }
 MYDEBUG(D_DIEHARD_OQSO){
   printf("%f %f %f\n",ptest.y,ptest.x,ptest.x-ptest.y);
 }

 Xtest_eval(&ptest);
 test[0]->pvalues[irun] = ptest.pvalue;

 MYDEBUG(D_DIEHARD_OQSO) {
   printf("# diehard_oqso(): ks_pvalue[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 /*
  * Don't forget to free w when done, in reverse order
  */
 for(i=0;i<32;i++){
   for(j=0;j<32;j++){
      for(k=0;k<32;k++){
         free(w[i][j][k]);
      }
      free(w[i][j]);
   }
   free(w[i]);
 }
 free(w);

}

