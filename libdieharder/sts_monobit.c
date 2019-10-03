
/*
* $Id: sts_monobit.c 237 2006-08-23 01:33:46Z rgb $
*
* See copyright in copyright.h and the accompanying file COPYING
* See also accompanying file STS.COPYING
*
*/

/*
 *========================================================================
 * This is a the monobit test, rewritten from the description in the
 * STS suite.
 *
 * Rewriting means that I can standardize the interface to gsl-encapsulated
 * routines more easily.  It also makes this my own code.
 *========================================================================
 */

#include <dieharder/libdieharder.h>

void sts_monobit(Test **test, int irun)
{

 int i,b,bsamples,bit;
 uint bitstring,blens,nbits;
 Xtest ptest;

 /*
  * ptest.x contains n_1's - n_0's = n_1's - (nbits - n_1's)
  *   or ptest.x = 2*n_1's - nbits;
  * ptest.y is the number we expect (2*n_1's = nbits, so ptest.y = 0)
  * ptest.sigma is the expected error, 1/sqrt(nbits).
  *
  * Note that the expected distribution is the "half normal" centered
  * on 0.0.  I need to figure out if this is responsible for the 1/sqrt(2)
  * in the pvalue = erfc(|y - x|/(sqrt(2)*sigma)).
  *
  * Another very useful thing to note is that we don't really need to
  * do "samples" here.  Or rather, we could -- for enough bits, the
  * distribution of means should be normal -- but we don't.
  *
  */
 /*
  * The number of bits per random integer tested.
  */
 blens = rmax_bits;
 nbits = blens*test[0]->tsamples;
 ptest.y = 0.0;
 ptest.sigma = sqrt((double)nbits);

 /*
  * NOTE WELL:  This can also be done by reading in a file!  Note
  * that if -b bits is specified, size will be "more than enough".
  */
 MYDEBUG(D_STS_MONOBIT) {
   printf("# rgb_bitdist(): Generating %u bits in bitstring",test[0]->tsamples*sizeof(uint)*8);
 }
 ptest.x = 0;
 for(i=0;i<test[0]->tsamples;i++) {
   bitstring = gsl_rng_get(rng);
   MYDEBUG(D_STS_MONOBIT) {
     printf("# rgb_bitdist(): rand_int[%d] = %u = ",i,bitstring);
     dumpbits(&bitstring,8*sizeof(uint));
   }
   for(b=0;b<blens;b++){
     /*
      * This gets the integer value of the ntuple at index position
      * n in the current bitstring, from a window with cyclic wraparound.
      */
     bit = get_bit_ntuple(&bitstring,1,1,b);
     ptest.x += bit;
   }
 }

 ptest.x = 2*ptest.x - nbits;
 MYDEBUG(D_STS_MONOBIT) {
   printf("mtext.x = %10.5f  ptest.sigma = %10.5f\n",ptest.x,ptest.sigma);
 }
 Xtest_eval(&ptest);
 test[0]->pvalues[irun] = ptest.pvalue;

 MYDEBUG(D_STS_MONOBIT) {
   printf("# sts_monobit(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

}

