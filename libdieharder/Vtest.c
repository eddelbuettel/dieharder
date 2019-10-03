/*
 *========================================================================
 * $Id: Vtest.c 223 2006-08-17 06:19:38Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * Vtest.c contains a set of routines for evaluating a p-value, the
 * probability that a given test result was obtained IF the underlying
 * random number generator was a "good" one (the null hypothesis), given a
 * vector x of observed results and a vector y of expected results.  It
 * determines the p-value using Pearson's chisq, which does not require
 * the independent input of the expected sigma for each "bin" (vector
 * position).
 *========================================================================
 */
#include <dieharder/libdieharder.h>

void Vtest_create(Vtest *vtest, unsigned int nvec)
{

 int i;
 if(verbose == D_BTEST || verbose == D_ALL){
   printf("Vtest_create(): Creating test struct for %u nvec.\n",nvec);
 }
 vtest->x = (double *) malloc(sizeof(double)*nvec);       /* sample results */
 vtest->y = (double *) malloc(sizeof(double)*nvec);       /* expected sample results */
 /* zero or set everything */
 for(i=0;i<nvec;i++){
   vtest->x[i] = 0.0;
   vtest->y[i] = 0.0;
 }
 vtest->nvec = nvec;
 vtest->chisq = 0.0;
 vtest->pvalue = 0.0;
 if(verbose == D_BTEST || verbose == D_ALL){
   printf("Vtest_create(): Created test structure for %d points.\n",vtest->nvec);
 }


}

void Vtest_destroy(Vtest *vtest)
{

 free(vtest->x);
 free(vtest->y);

}

void Vtest_eval(Vtest *vtest)
{

 uint i,ndof;
 double delchisq,chisq;
 double x_tot,y_tot;


 /*
  * This routine evaluates chi-squared, where:
  * vtest->x is the trial vector
  * vtest->y is the exact vector
  * vtest->sigma is the vector of expected error for y
  *              (for the exact/true distribution)
  * nvec is the vector length(s).
  *
  * x, y, sigma all must be filled in my the calling routine.
  *
  * We'll have to see how this handles sigma[i] = 0.0.  Probably badly
  * but smoothly.
  */
 /* verbose=1; */
 if(verbose == D_BTEST || verbose == D_ALL){
   printf("Evaluating chisq and pvalue for %d points\n",vtest->nvec);
 }

 chisq = 0.0;
 x_tot = 0.0;
 y_tot = 0.0;
 ndof = 0;
 if(verbose == D_BTEST || verbose == D_ALL){
   printf("# %7s   %3s      %3s %10s      %10s %10s %9s\n",
           "bit/bin","DoF","X","Y","sigma","del-chisq","chisq");
   printf("#==================================================================\n");
 }
 for (i=0;i<vtest->nvec;i++) {
   if(vtest->x[i] > 10.0){
     x_tot += vtest->x[i];
     y_tot += vtest->y[i];
     delchisq = (vtest->x[i] - vtest->y[i])*(vtest->x[i] - vtest->y[i])/vtest->y[i];
     /*  Alternative way of evaluating chisq for binomial only.
     delchisq = (vtest->x[i] - vtest->y[i])/vtest->sigma[i];
     delchisq *= delchisq;
     */
     chisq += delchisq;
     if(verbose == D_BTEST || verbose == D_ALL){
       printf("# %5u\t%3u\t%12.4f\t%12.4f\t%8.4f\t%10.4f\n",
                  i,ndof,vtest->x[i],vtest->y[i],delchisq,chisq);
     }
     ndof++;
   }
 }

 if(verbose == D_BTEST || verbose == D_ALL){
   printf("Total:  %10.4f  %10.4f\n",x_tot,y_tot);
   printf("#==================================================================\n");
   printf("Evaluated chisq = %f for %u degrees of freedom\n",chisq,ndof);
 }
 vtest->chisq = chisq;

 /*
  * Now evaluate the corresponding pvalue.  The only real question
  * is what is the correct number of degrees of freedom.  I'd argue we
  * did use a constraint when we set expected = binomial*nsamp, so we'll
  * go for ndof (count of nvec tallied) - 1.
  */
 ndof--;
 vtest->pvalue = gsl_sf_gamma_inc_Q((double)(ndof)/2.0,chisq/2.0);
 if(verbose == D_BTEST || verbose == D_ALL){
   printf("Evaluted pvalue = %6.4f in Vtest_eval().\n",vtest->pvalue);
 }
 /* verbose=0; */

}

