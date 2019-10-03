/*
 * $Id: chisq.c 229 2006-08-21 20:33:07Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This code evaluates a vector of chisq's and p-values from a vector
 * of sampled results.  I believe that it assumes a Poissonian
 * distribution in the vector, not a normal one.  If so, we'll rename
 * this appropriately.
 *========================================================================
 */


#include <dieharder/libdieharder.h>


/*
 * This routine computes chisq on a vector of nvec values drawn
 * presumably from a known discrete distribution (Poisson).
 * lambda is the expected mean, and pvec is a vector
 * where a list of associated p-values is returned.
 *
 * Note that a Poisson distribution with mean lambda is:
 *
 *  p(k) = (lambda^k/k!) exp(-lambda)
 *
 * and that it can be computed in a single pass by:
 *
 *  p(k) = gsl_ran_poisson_pdf(uint k,double lambda);
 *
 * All we need to do, then, is compute p(k) and use it to
 * determine a vector of estimates for the interval frequency,
 * then compute Pearson's chisq in a straightforward way.
 * The only "tricky" part of all of this is going to be a)
 * scaling the data by the number of independent samples; and
 * b) converting into p-values, which requires a knowledge of
 * the number of degrees of freedom of the fit.
 */

double chisq_poisson(uint *observed,double lambda,int kmax,uint nsamp)
{

 uint i,j,k;
 double *expected;
 double delchisq,chisq,pvalue;

 /*
  * Allocate a vector for the expected value of the bin frequencies up
  * to kmax-1.
  */
 expected = (double *)malloc(kmax*sizeof(double));
 for(k = 0;k<kmax;k++){
   expected[k] = nsamp*gsl_ran_poisson_pdf(k,lambda);
 }

 /*
  * Compute Pearson's chisq for this vector of the data with poisson
  * expected values.
  */
 chisq = 0.0;
 for(k = 0;k < kmax;k++){
   delchisq = ((double) observed[k] - expected[k])*
      ((double) observed[k] - expected[k])/expected[k];
   chisq += delchisq;
   if(verbose == D_CHISQ || verbose == D_ALL){
     printf("%u:  observed = %f,  expected = %f, delchisq = %f, chisq = %f\n",
        k,(double)observed[k],expected[k],delchisq,chisq);
   }
 }

 if(verbose == D_CHISQ || verbose == D_ALL){
   printf("Evaluated chisq = %f for %u k values\n",chisq,kmax);
 }

 /*
  * Now evaluate the corresponding pvalue.  The only real question
  * is what is the correct number of degrees of freedom.  We have
  * kmax bins, so it should be kmax-1.
  */
 pvalue = gsl_sf_gamma_inc_Q((double)(kmax-1)/2.0,chisq/2.0);
 if(verbose == D_CHISQ || verbose == D_ALL){
   printf("pvalue = %f in chisq_poisson.\n",pvalue);
 }

 free(expected);

 return(pvalue);

}

double chisq_pearson(double *observed,double *expected,int kmax)
{

 uint i,j,k;
 double delchisq,chisq,pvalue;

 /*
  * Compute Pearson's chisq for this vector of the data.
  */
 chisq = 0.0;
 for(k = 0;k < kmax;k++){
   delchisq = (observed[k] - expected[k])*
      (observed[k] - expected[k])/expected[k];
   chisq += delchisq;
   if(verbose){
     printf("%u:  observed = %f,  expected = %f, delchisq = %f, chisq = %f\n",
        k,observed[k],expected[k],delchisq,chisq);
   }
 }

 if(verbose){
   printf("Evaluated chisq = %f for %u k values\n",chisq,kmax);
 }

 /*
  * Now evaluate the corresponding pvalue.  The only real question
  * is what is the correct number of degrees of freedom.  We have
  * kmax bins, so it should be kmax-1.
  */
 pvalue = gsl_sf_gamma_inc_Q((double)(kmax-1)/2.0,chisq/2.0);
 if(verbose){
   printf("pvalue = %f in chisq_pearson.\n",pvalue);
 }

 return(pvalue);

}

double chisq_binomial(double *observed,double prob,uint kmax,uint nsamp)
{

 uint n,nmax,ndof;
 double expected,delchisq,chisq,pvalue,bprob,obstotal,exptotal;

 chisq = 0.0;
 obstotal = 0.0;
 exptotal = 0.0;
 ndof = 0;
 nmax = kmax;
 if(verbose){
   printf("# %7s   %3s      %3s %10s      %10s %9s\n",
           "bit/bin","DoF","X","Y","del-chisq","chisq");
   printf("#==================================================================\n");
 }
 for(n = 0;n <= nmax;n++){
   if(observed[n] > 10.0){
     expected = nsamp*binomial(nmax,n,prob);
     obstotal += observed[n];
     exptotal += expected;
     delchisq = (observed[n] - expected)*(observed[n] - expected)/expected;
     chisq += delchisq;
     if(verbose){
       printf("# %5u     %3u   %10.4f %10.4f %10.4f %10.4f\n",
                  n,ndof,observed[n],expected,delchisq,chisq);
     }
     ndof++;
   }
 }
 if(verbose){
   printf("Total:  %10.4f  %10.4f\n",obstotal,exptotal);
   printf("#==================================================================\n");
   printf("Evaluated chisq = %f for %u degrees of freedom\n",chisq,ndof);
 }

 /*
  * Now evaluate the corresponding pvalue.  The only real question
  * is what is the correct number of degrees of freedom.  I'd argue we
  * did use a constraint when we set expected = binomial*nsamp, so we'll
  * go for ndof (count of bins tallied) - 1.
  */
 ndof--;
 pvalue = gsl_sf_gamma_inc_Q((double)(ndof)/2.0,chisq/2.0);
 if(verbose){
   printf("Evaluted pvalue = %6.4f in chisq_binomial.\n",pvalue);
 }

 return(pvalue);

}

