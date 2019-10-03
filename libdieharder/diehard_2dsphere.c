/*
 * $Id: diehard_2dsphere.c 231 2006-08-22 16:18:05Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 * This is the Diehard minimum distance (2d sphere) test, rewritten from 
 * the description in tests.txt on George Marsaglia's diehard site.
 *
 *               THE MINIMUM DISTANCE TEST                       ::
 * It does this 100 times::   choose n=8000 random points in a   ::
 * square of side 10000.  Find d, the minimum distance between   ::
 * the (n^2-n)/2 pairs of points.  If the points are truly inde- ::
 * pendent uniform, then d^2, the square of the minimum distance ::
 * should be (very close to) exponentially distributed with mean ::
 * .995 .  Thus 1-exp(-d^2/.995) should be uniform on [0,1) and  ::
 * a KSTEST on the resulting 100 values serves as a test of uni- ::
 * formity for random points in the square. Test numbers=0 mod 5 ::
 * are printed but the KSTEST is based on the full set of 100    ::
 * random choices of 8000 points in the 10000x10000 square.      ::
 *
 *                      Comment
 * Obviously the same test as 3d Spheres but in 2d, hence the
 * name.
 *========================================================================
 */


#include <dieharder/libdieharder.h>

void diehard_2dsphere(Test **test, int irun)
{

 int i,j,k,l,m;
 C3_2D *c3;
 double r1,r2,r3,rmin,r2min;

 /*
  * This one should be pretty straightforward.  Generate a vector
  * of two random coordinates in the range 0-10000.  Do
  * a simple double loop through to float the smallest separation out.
  * Generate p, save in a sample vector.  Apply KS test.
  */
 c3 = (C3_2D *)malloc(POINTS_2D*sizeof(C3_2D));

 rmin = 2000.0;
 for(j=0;j<POINTS_2D;j++){
   /*
    * Generate a new point in the 10000x10000 square.
    */
   for(k=0;k<DIM_2D;k++) c3[j].x[k] = 10000.0*gsl_rng_uniform_pos(rng);
   if(verbose == D_DIEHARD_2DSPHERE || verbose == D_ALL){
     printf("%d: (%8.2f,%8.2f)\n",j,c3[j].x[0],c3[j].x[1]);
   }

   /*
    * Now compute the distance between the new point and all previously
    * picked points.
    */
   for(k=j-1;k>=0;k--){
     r2 = 0.0;
     for(l=0;l<DIM_2D;l++){
       r2 += (c3[j].x[l]-c3[k].x[l])*(c3[j].x[l]-c3[k].x[l]);
     }
     r1 = sqrt(r2);
     if(r1<rmin) {
       rmin = r1;
       r2min = r2;
     }
   }
 }

 MYDEBUG(D_DIEHARD_2DSPHERE) {
   printf("Found rmin = %f  (r^2 = %f)\n",rmin,r2min);
 }

 test[0]->pvalues[irun] = 1.0 - exp(-r2min/0.995);

 MYDEBUG(D_DIEHARD_2DSPHERE) {
   printf("# diehard_2dsphere(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

}

