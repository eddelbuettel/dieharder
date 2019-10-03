/*
 * $Id: diehard_dna.c 230 2006-08-22 05:31:54Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard DNA test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 *   The DNA test considers an alphabet of 4 letters::  C,G,A,T,::
 * determined by two designated bits in the sequence of random   ::
 * integers being tested.  It considers 10-letter words, so that ::
 * as in OPSO and OQSO, there are 2^20 possible words, and the   ::
 * mean number of missing words from a string of 2^21  (over-    ::
 * lapping)  10-letter  words (2^21+9 "keystrokes") is 141909.   ::
 * The standard deviation sigma=339 was determined as for OQSO   ::
 * by simulation.  (Sigma for OPSO, 290, is the true value (to   ::
 * three places), not determined by simulation.                  ::
 *
 * Note 2^21 = 2097152
 * The tests BITSTREAM, OPSO, OQSO and DNA are all closely related.
 *========================================================================
 */


#include <dieharder/libdieharder.h>
#include "static_get_bits.c"
static uint mask;

void diehard_dna(Test **test, int irun)
{

 uint i,j,k,l,m,n,o,p,q,r,t,boffset;
 uint i0,j0,k0,l0,m0,n0,o0,p0,q0,r0;
 Xtest ptest;
 char **********w;

 /*
  * p = 141909, with sigma 339, FOR tsamples 2^21+1 2 letter words.
  * These cannot be varied unless one figures out the actual
  * expected "missing works" count as a function of sample size.  SO:
  *
  * ptest.x = number of "missing words" given 2^21+1 trials
  * ptest.y = 141909
  * ptest.sigma = 339
  */
 ptest.x = 0.0;  /* Initial value */
 ptest.y = 141909.0;
 ptest.sigma = 339.0;

 /*
  * We now make tsamples measurements, as usual, to generate the missing
  * statistic.  Wow!  10 dimensions!  I don't think even my tensor()
  * package will do that...;-)
  */

 w = (char **********)malloc(4*sizeof(char *********));
 for(i=0;i<4;i++){
   w[i] = (char *********)malloc(4*sizeof(char ********));
   for(j=0;j<4;j++){
     w[i][j] = (char ********)malloc(4*sizeof(char *******));
     for(k=0;k<4;k++){
       w[i][j][k] = (char *******)malloc(4*sizeof(char******));
       for(l=0;l<4;l++){
         w[i][j][k][l] = (char ******)malloc(4*sizeof(char*****));
         for(m=0;m<4;m++){
           w[i][j][k][l][m] = (char *****)malloc(4*sizeof(char****));
           for(n=0;n<4;n++){
             w[i][j][k][l][m][n] = (char ****)malloc(4*sizeof(char***));
             for(o=0;o<4;o++){
               w[i][j][k][l][m][n][o] = (char ***)malloc(4*sizeof(char**));
               for(p=0;p<4;p++){
                 w[i][j][k][l][m][n][o][p] = (char **)malloc(4*sizeof(char*));
                 for(q=0;q<4;q++){
                   w[i][j][k][l][m][n][o][p][q] = (char *)malloc(4*sizeof(char));
                   /* Zero the column */
                   memset(w[i][j][k][l][m][n][o][p][q],0,4*sizeof(char));
		 }
	       }
	     }
	   }
	 }
       }
     }
   }
 }

 /*
  * To minimize the number of rng calls, we use each j and k mod 32
  * to determine the offset of the 10-bit long string (with
  * periodic wraparound) to be used for the next iteration.  We
  * therefore have to "seed" the process with a random l
  */
 q = gsl_rng_get(rng);
 /* mask = ((2 << 1)-1); */
 mask = 0x3;
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
       m0 = gsl_rng_get(rng);
       n0 = gsl_rng_get(rng);
       o0 = gsl_rng_get(rng);
       p0 = gsl_rng_get(rng);
       q0 = gsl_rng_get(rng);
       r0 = gsl_rng_get(rng);
       boffset = 0;
     }
     /*
      * Get four "letters" (indices into w)
      */
     i = get_bit_ntuple_from_uint(i0,2,mask,boffset);
     j = get_bit_ntuple_from_uint(j0,2,mask,boffset);
     k = get_bit_ntuple_from_uint(k0,2,mask,boffset);
     l = get_bit_ntuple_from_uint(l0,2,mask,boffset);
     m = get_bit_ntuple_from_uint(m0,2,mask,boffset);
     n = get_bit_ntuple_from_uint(n0,2,mask,boffset);
     o = get_bit_ntuple_from_uint(o0,2,mask,boffset);
     p = get_bit_ntuple_from_uint(p0,2,mask,boffset);
     q = get_bit_ntuple_from_uint(q0,2,mask,boffset);
     r = get_bit_ntuple_from_uint(r0,2,mask,boffset);
     /* printf("%u:   %u  %u  %u  %u  %u\n",t,i,j,k,l,boffset); */
     w[i][j][k][l][m][n][o][p][q][r]++;
     boffset++;

   } else {
     /*
      * Get two "letters" (indices into w)
      */
     boffset = q%32;
     i = gsl_rng_get(rng);
     i = get_bit_ntuple_from_uint(i,2,mask,boffset);
     boffset = i%32;
     j = gsl_rng_get(rng);
     j = get_bit_ntuple_from_uint(j,2,mask,boffset);
     boffset = j%32;
     k = gsl_rng_get(rng);
     k = get_bit_ntuple_from_uint(k,2,mask,boffset);
     boffset = k%32;
     l = gsl_rng_get(rng);
     l = get_bit_ntuple_from_uint(l,2,mask,boffset);
     boffset = l%32;
     m = gsl_rng_get(rng);
     m = get_bit_ntuple_from_uint(m,2,mask,boffset);
     boffset = m%32;
     n = gsl_rng_get(rng);
     n = get_bit_ntuple_from_uint(n,2,mask,boffset);
     boffset = n%32;
     o = gsl_rng_get(rng);
     o = get_bit_ntuple_from_uint(o,2,mask,boffset);
     boffset = o%32;
     p = gsl_rng_get(rng);
     p = get_bit_ntuple_from_uint(p,2,mask,boffset);
     boffset = p%32;
     q = gsl_rng_get(rng);
     q = get_bit_ntuple_from_uint(q,2,mask,boffset);
     boffset = q%32;
     r = gsl_rng_get(rng);
     r = get_bit_ntuple_from_uint(r,2,mask,boffset);
     w[i][j][k][l][m][n][o][p][q][r]++;
   }
 }
 /*
  * Now we count the holes, so to speak
  */
 ptest.x = 0;
 for(i=0;i<4;i++){
   for(j=0;j<4;j++){
     for(k=0;k<4;k++){
       for(l=0;l<4;l++){
         for(m=0;m<4;m++){
           for(n=0;n<4;n++){
             for(o=0;o<4;o++){
               for(p=0;p<4;p++){
                 for(q=0;q<4;q++){
                   for(r=0;r<4;r++){
                     if(w[i][j][k][l][m][n][o][p][q][r] == 0){
                       ptest.x += 1.0;
                       /* printf("ptest.x = %f  Hole: w[%u][%u][%u][%u] = %u\n",ptest.x,i,j,k,l,w[i][j][k][l]); */
		     }
		   }
		 }
	       }
	     }
	   }
	 }
       }
     }
   }
 }
 MYDEBUG(D_DIEHARD_DNA) {
   printf("%f %f %f\n",ptest.y,ptest.x,ptest.x-ptest.y);
 }

 Xtest_eval(&ptest);
 test[0]->pvalues[irun] = ptest.pvalue;

 MYDEBUG(D_DIEHARD_DNA) {
   printf("# diehard_dna(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 /*
  * Don't forget to free w when done, in reverse order
  */
 for(i=0;i<4;i++){
   for(j=0;j<4;j++){
     for(k=0;k<4;k++){
       for(l=0;l<4;l++){
         for(m=0;m<4;m++){
           for(n=0;n<4;n++){
             for(o=0;o<4;o++){
               for(p=0;p<4;p++){
                 for(q=0;q<4;q++){
                   free(w[i][j][k][l][m][n][o][p][q]);
		 }
		 free(w[i][j][k][l][m][n][o][p]);
	       }
               free(w[i][j][k][l][m][n][o]);
	     }
             free(w[i][j][k][l][m][n]);
	   }
           free(w[i][j][k][l][m]);
	 }
         free(w[i][j][k][l]);
       }
       free(w[i][j][k]);
     }
     free(w[i][j]);
   }
   free(w[i]);
 }
 free(w);

}

