/*
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 * rgb_bitdist is a non-overlapping-sample complement to sts_serial.
 * Because it uses non-overlapping samples, its logic is a bit simpler
 * and it doesn't have to mess with covariance.  It can more or less
 * directly perform chisq tests on the accumulated bitpattern frequencies
 * to see if they are multinomially distributed.  See following
 * description:
 *
 * This is a test that checks to see if the rng generates bit patterns
 * (n-tuples) that are distributed correctly (multinomially).  For example,
 * for 2-tuples (bit pairs) there are four possibilities: 00, 01, 10, 11.
 * Each should occur with a probability of 1/4, hence the count of (say) K
 * 00 bitpairs out of N trials should be distributed (over M samples)
 * according to the binomial probability p = binomial(N,K,0.25) -- that
 * is, the expected count for k 00's is M*p(N,K,0.25).
 *========================================================================
 */

/*
 *                      Non-Overlapping Test
 *==================================================================
 * This test consumes 100*2^n rands for any given size ntuple n.
 * This makes the expectation value for all the frequencies a fixed
 * 100 independent of probability, which is convenient.
 *
 * It can in principle generate two statistics.  First is a simple
 * chisq test on the entire frequency vector with p = 1/2^n and 2^n-1
 * degrees of freedom.  This should reveal "direct" bias in the
 * sample, e.g. any bit pattern or set of bit patterns that occurs
 * more or less frequently than one expects from the multinomial
 * problem symmetry.  Basically this ensures that the rng produces
 * ntuples uniformly, on average, where obviously any failure to
 * do so is a bad, bad thing.
 *
 * The second one is to look at the actual distribution of outcomes
 * per ntuple in a MATRIX indexed by the ntuple.  That is, for 2-bit
 * patterns 00 01 10 11, each with expected count 100 (from 400
 * samples pulled from the rng) generated psamples = 100 times, one
 * might find that one gets 100 exactly 18 times, 101 16, 99 14, 102 13,
 * 98 15, etc. -- in other words there should be a BINOMIAL
 * distribution of frequencies over many trials.
 */

#include <dieharder/libdieharder.h>

#include "static_get_bits.c"

/*
 * This is a buffer of uint length (2^nb)+1 that we will fill with rands
 * to be tested, once per test.
 */

void sts_serial(Test **test,int irun)
{

 uint bsize;       /* number of bits/samples in uintbuf */
 uint nb,nb1;          /* number of bits in a tested ntuple */
 uint value;       /* value of sampled ntuple (as a uint) */
 uint mask;        /* mask in only nb bits */
 uint bi;          /* bit offset relative to window */

 /* Look for cruft below */

 uint i,j,n,m,t;            /* generic loop indices */
 uint ctotal;  /* count of any ntuple per bitstring */
 double **freq,*psi2,*delpsi2,*del2psi2;
 double pvalue;
 uint *uintbuf;
 uint window;  /* uint window into uintbuf, slide along a byte at at time. */

 double mono_mean,mono_sigma;  /* For single bit test */

 /*
  * Sample a bitstring of rgb_bitstring_ntuple in length (exactly).  Note
  * that I'm going to force nb>=2, nb<=16 for the moment.  The routine
  * might "work" for nb up to 24 or even (on a really big memory machine)
  * 32, but memory requirements in uints are around 2^(nb+1) + n where n
  * is order unity, maybe worse, so I think it would take a 12 to 16 GB
  * machine to be able to just hold the memory, and then it would take a
  * very long time.  However, the testing process might parallelize
  * decently if we could farm out the blocks of bits to be run efficiently
  * over a network relative to the time required to generate them.
  */
 nb = 16;        /* Just ignore sts_serial_ntuple for now */
 tsamples = test[0]->tsamples;  /* ditto */
 MYDEBUG(D_STS_SERIAL){
   printf("#==================================================================\n");
   printf("# Starting sts_serial.\n");
   printf("# sts_serial: Testing ntuple = %u\n",nb);
 }
 /*
  * This is useful as a limit here and there as we have things that range
  * from 0 to nb INclusive (unless/until we fix the mallocs up a bit).
  */
 nb1 = nb+1;

 /*
  * We need a vector of freq vectors.  We make each frequency counter
  * vector only as large as it needs to be.  We zero each accumulator as
  * we make it.  Note that we cannot sample "0 bits at a time" so m starts
  * at 1.
  */
 freq = (double **) malloc(nb1*sizeof(double *));
 for(m = 1;m < nb1;m++) {
   freq[m] = (double *)malloc(pow(2,m)*sizeof(double));
   memset(freq[m],0,pow(2,m)*sizeof(double));
 }

 /*
  * These are the statistics required by sts_serial in SP800.  psi2[m] is
  * basically the sum of the squares of the bitpattern frequences for a
  * given number of bits m.  delpsi2 is the difference between psi2[m] and
  * psi2[m-1].  del2psi2 is the difference of delpsi2[m] and delpsi2[m-1].
  * We zero only need to zero psi2, as the other two are set directly.
  *
  * Note that we can only generate psi2 for m>0, delpsi2 for m>1, delpsi3
  * for m>2.  We can therefore only generate both test statistics (based
  * on delpsi2 and del2psi2) for m>=3 to m=16.  What we CAN do for m=1
  * is generate a straight normal p-value on a binomial probability for
  * e.g. average number of 1's, basically sts_monobit.  That would give
  * us one test for m = 1, one test for m = 2, and two tests each for
  * m = [3,16].
  */
 psi2     = (double *) malloc(nb1*sizeof(double));
 delpsi2  = (double *) malloc(nb1*sizeof(double));
 del2psi2 = (double *) malloc(nb1*sizeof(double));
 memset(psi2,0,nb1*sizeof(double));

 /*
  * uintbuf is the one true vector of rands processed during this test.
  * It needs to be at LEAST 2^16 = 64K uints in length, 2^20 = 1M is much
  * better.  We'll default this from tsamples == 100000, though; it doesn't
  * have to be divisible by 2 or anything.  Note that we add one to tsamples
  * to permit cyclic wraparound of the overlapping samples, although honestly
  * this hardly matters in the limit of large tsamples.
  */
 uintbuf = (uint *)malloc((tsamples+1)*sizeof(uint));

 /*
  * If uintbuf[test[0]->tsamples] is allocated (plus one for wraparound)
  * then we need to count the number of bits, which is the number of
  * OVERLAPPING samples we will pull.
  */
 bsize = tsamples*sizeof(uint)*CHAR_BIT;
 MYDEBUG(D_STS_SERIAL){
   printf("# sts_serial(): bsize = %u\n",bsize);
 }

 /*
  * We start by filling testbuf with rands and cloning the first into
  * the last slot for cyclic wrap.  This initial effort just uses the
  * fast gsl_rng_get() call, but we'll need to use the inline from the
  * static_get_rng routines in production.
  */
 for(t=0;t<tsamples;t++){
   /* A bit slower per call, but won't fail for short rngs */
   uintbuf[t] = get_rand_bits_uint(32,0xFFFFFFFF,rng);
   /* Fast, but deadly to rngs with less than 32 bits returned */
   /* uintbuf[t] = gsl_rng_get(rng); */
   MYDEBUG(D_STS_SERIAL){
     printf("# sts_serial(): %u:  ",t);
     dumpuintbits(&uintbuf[t],1);
     printf("\n");
   }
 }
 uintbuf[tsamples] = uintbuf[0];   /* Periodic wraparound */
 MYDEBUG(D_STS_SERIAL){
   printf("# sts_serial(): %u:  ",tsamples);
   dumpuintbits(&uintbuf[tsamples],1);
   printf("\n");
 }

 /*
  * We now in ONE PASS loop over all of the possible values of m from
  * 1 to 16.
  */
 for(m=1;m<nb1;m++){
 
   /*
    * Set the mask for bits to be returned.  This will work fine for
    * m in the allowed range, and will fail if m = 32 if that ever
    * happens in the future.
    */
   mask = 0;
   for(i = 0; i < m; i++){
      mask |= (1u << m);
   }
   mask = ((1u << m) - 1);

   /* Initialize index and count total */
   j = 0;
   ctotal = 0;
   MYDEBUG(D_STS_SERIAL){
     printf("looping bsize = %u times\n",bsize);
   }
   for(t=0;t<bsize;t++){
     /*
      * Offset of current sample, relative to left boundary of window.
      */
     if((t%32) == 0){
       window = uintbuf[j];  /* start with window = testbuf = charbuf */
       MYDEBUG(D_STS_SERIAL){
         printf("uint window[%u] = %08x = ",j,window);
         dumpuintbits(&window,1);
         printf("\n");
       }
       j++;
     }
     bi = t%16;
     value = (window >> (32 - m - bi)) & mask;
     MYDEBUG(D_STS_SERIAL){
       dumpbitwin(value,m);
       printf("\n");
     }
     freq[m][value]++;
     ctotal++;
     if(bi == 15){
       window = (window << 16);
       window += (uintbuf[j] >> 16);
       MYDEBUG(D_STS_SERIAL){
         printf("half window[%u] = %08x = ",j,window);
         dumpuintbits(&window,1);
         printf("\n");
       }
     }
   }

   MYDEBUG(D_STS_SERIAL){
     printf("# sts_serial():=====================================================\n");
     printf("# sts_serial():                  Count table\n");
     printf("# sts_serial():\tbits\tvalue\tcount\tprob\n");
     for(i = 0; i<pow(2,m); i++){
       printf("# sts_serial():   ");
       dumpbitwin(i,m);
       printf("\t%u\t%f\t%f\n",i,freq[m][i],(double) freq[m][i]/ctotal);
     }
     printf("# sts_serial(): Total count = %u, target probability = %f\n",ctotal,1.0/pow(2,m));
   }

 } /* End of m loop */

 /*
  * Now it is time to implement the statistic from STS SP800 whatever.
  */
 MYDEBUG(D_STS_SERIAL){
   printf("# sts_serial():=====================================================\n");
 }
 for(m=1;m<nb1;m++){
   for(i=0;i<pow(2,m);i++){
     psi2[m] += freq[m][i]*freq[m][i];
     /* printf("freq[%u][%u] = %f  p2 = %f \n",m,i,freq[m][i],psi2[m]); */
   }
   psi2[m] = pow(2,m)*psi2[m]/bsize - bsize;
   MYDEBUG(D_STS_SERIAL){
     printf("# sts_serial(): psi2[%u] = %f\n",m,psi2[m]);
   }
 }

 j=0;
 /*
  * This is sts_monobit, basically.
  */
 mono_mean = (double) 2*freq[1][0] - bsize;   /* Should be 0.0 */
 mono_sigma = sqrt((double)bsize);
 /* printf("mono mean = %f   mono_sigma = %f\n",mono_mean,mono_sigma); */
 test[j++]->pvalues[irun] = gsl_cdf_gaussian_P(mono_mean,mono_sigma);

 for(m=2;m<nb1;m++){
   delpsi2[m] = psi2[m] - psi2[m-1];
   del2psi2[m] = psi2[m] - 2.0*psi2[m-1] + psi2[m-2];
   pvalue = gsl_sf_gamma_inc_Q(pow(2,m-2),delpsi2[m]/2.0);
   test[j++]->pvalues[irun] = pvalue;
   MYDEBUG(D_STS_SERIAL){
     printf("pvalue 1[%u] = %f\n",m,pvalue);
   }
   if(m>2){
     pvalue = gsl_sf_gamma_inc_Q(pow(2,m-3),del2psi2[m]/2.0);
     test[j++]->pvalues[irun] = pvalue;
     MYDEBUG(D_STS_SERIAL){
       printf("pvalue 2[%u] = %f\n",m,pvalue);
     }
   }
 }

 free(uintbuf);
 free(psi2);
 free(del2psi2);
 free(delpsi2);
 for(m = 1;m < nb1;m++){
   free(freq[m]);
 }
 free(freq);

}
