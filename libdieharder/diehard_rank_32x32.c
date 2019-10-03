/*
* $Id: diehard_rank_32x32.c 230 2006-08-22 05:31:54Z rgb $
*
* See copyright in copyright.h and the accompanying file COPYING
*
*/

/*
 *========================================================================
 * This is the Diehard BINARY RANK 31x31 test, rewritten from the 
 * description in tests.txt on George Marsaglia's diehard site.
 *
 * This is the BINARY RANK TEST for 31x31 matrices. The leftmost ::
 * 31 bits of 31 random integers from the test sequence are used ::
 * to form a 31x31 binary matrix over the field {0,1}. The rank  ::
 * is determined. That rank can be from 0 to 31, but ranks< 28   ::
 * are rare, and their counts are pooled with those for rank 28. ::
 * Ranks are found for 40,000 such random matrices and a chisqua-::
 * re test is performed on counts for ranks 31,30,29 and <=28.   ::
 *
 *                          Comments
 *========================================================================
 */


#include <dieharder/libdieharder.h>

void diehard_rank_32x32(Test **test, int irun)
{

 int i,j,k,t,rank;
 double r,smax,s;
 uint bitstring,rmask,mask;
 Vtest vtest;

 Vtest_create(&vtest,33,"diehard_rank_32x32",gsl_rng_name(rng));
 for(i=0;i<29;i++){
   vtest.x[0] = 0.0;
   vtest.y[0] = 0.0;
 }
 vtest.x[29] = 0.0;
 vtest.y[29] = test[0]->tsamples*0.0052854502e+00;
 vtest.x[30] = 0.0;
 vtest.y[30] = test[0]->tsamples*0.1283502644e+00;
 vtest.x[31] = 0.0;
 vtest.y[31] = test[0]->tsamples*0.5775761902e+00;
 vtest.x[32] = 0.0;
 vtest.y[32] = test[0]->tsamples*0.2887880952e+00;
 
 for(t=0;t<test[0]->tsamples;t++){

   /*
    * We generate 32 x 32 random integers and put their leftmost
    * (most significant) bit into each bit slot of the diehard_rank_32x32_mtx.
    */
   if(verbose == D_DIEHARD_RANK_32x32 || verbose == D_ALL){
     printf("# diehard_rank_32x32(): Input random matrix = \n");
   }
   for(i=0;i<32;i++){
     if(verbose == D_DIEHARD_RANK_32x32 || verbose == D_ALL){
       printf("# ");
     }
     diehard_rank_32x32_mtx[i][0] = 0;
     for(j=0;j<32;j++){
       bitstring = gsl_rng_get(rng);
       bitstring = get_bit_ntuple(&bitstring,1,1,31);
       diehard_rank_32x32_mtx[i][0] <<= 1;
       diehard_rank_32x32_mtx[i][0] += bitstring;
       mask <<= 1;
     }
     if(verbose == D_DIEHARD_RANK_32x32 || verbose == D_ALL){
       dumpbits(diehard_rank_32x32_mtx[i],32);
     }
   }

   rank = binary_rank(diehard_rank_32x32_mtx,32,32);
   if(verbose == D_DIEHARD_RANK_32x32 || verbose == D_ALL){
     printf("binary rank = %d\n",rank);
   }

   if(rank <= 29){
     vtest.x[29]++;
   } else {
     vtest.x[rank]++;
   }
 }

 /* for(i=0;i<33;i++) printf("vtest.x[%d] =  %f\n",i,vtest.x[i]); */

 Vtest_eval(&vtest);
 test[0]->pvalues[irun] = vtest.pvalue;
 MYDEBUG(D_DIEHARD_RANK_32x32) {
   printf("# diehard_rank_32x32(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 Vtest_destroy(&vtest);

}

