/*
 * $Id: diehard_bitstream.c 185 2006-07-12 22:23:59Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard sums test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 *             The  OVERLAPPING SUMS test                        ::
 * Integers are floated to get a sequence U(1),U(2),... of uni-  ::
 * form [0,1) variables.  Then overlapping sums,                 ::
 *   S(1)=U(1)+...+U(100), S2=U(2)+...+U(101),... are formed.    ::
 * The S's are virtually normal with a certain covariance mat-   ::
 * rix.  A linear transformation of the S's converts them to a   ::
 * sequence of independent standard normals, which are converted ::
 * to uniform variables for a KSTEST. The  p-values from ten     ::
 * KSTESTs are given still another KSTEST.                       ::
 *
 *                       Comments
 *========================================================================
 */


#include <dieharder/libdieharder.h>

double f[] = {
    0.0000,
    0.0017, 0.0132, 0.0270, 0.0406, 0.0538,
    0.0665, 0.0787, 0.0905, 0.1020, 0.1133,
    0.1242, 0.1349, 0.1454, 0.1557, 0.1659,
    0.1760, 0.1859, 0.1957, 0.2054, 0.2150,
    0.2246, 0.2341, 0.2436, 0.2530, 0.2623,
    0.2716, 0.2809, 0.2902, 0.2995, 0.3087,
    0.3180, 0.3273, 0.3366, 0.3459, 0.3552,
    0.3645, 0.3739, 0.3833, 0.3928, 0.4023,
    0.4118, 0.4213, 0.4309, 0.4406, 0.4504,
    0.4602, 0.4701, 0.4800, 0.4900, 0.5000,
    0.5100, 0.5199, 0.5299, 0.5397, 0.5495,
    0.5593, 0.5690, 0.5787, 0.5882, 0.5978,
    0.6073, 0.6167, 0.6260, 0.6354, 0.6447,
    0.6540, 0.6632, 0.6724, 0.6817, 0.6910,
    0.7003, 0.7096, 0.7189, 0.7282, 0.7375,
    0.7468, 0.7562, 0.7657, 0.7752, 0.7848,
    0.7944, 0.8041, 0.8140, 0.8239, 0.8340,
    0.8442, 0.8545, 0.8650, 0.8757, 0.8867,
    0.8980, 0.9095, 0.9214, 0.9337, 0.9464,
    0.9595, 0.9731, 0.9868, 0.9983, 1.0000};

void diehard_sums(Test **test, int irun)
{

 int i,j,k,m,t;
 double a,b,qq,*x,*y,*rand_dbl;
 double pvalue,h;
 Xtest ptest,ptmp;

 /*
  * SUMS requires that we evaluate y[i] = \sum_i^{100+i} u[i].  Naturally,
  * the sum (or mean, if you prefer) is supposedly normally distributed
  * according to the central limit theorem.  So each number has an
  * associated p-value -- the probability of that particular number
  * occurring from the random sum.  The vector of p-values can be turned
  * into an overall p-value via a KS test.
  *
  * This appears to be yet another variant on the nilpotent Markov
  * chain tests, like those Janusz Kawczak has communicated with me
  * about (see paper in docs).  In principle if I figure out one of
  * these tests I can figure them all out, and maybe see how to fix
  * them, as I strongly suspect that they are broken and/or need an
  * update as far as the numbers in the covariance matrices are
  * concerned.
  *
  * Things get all funky then in Marsaglia's description, at least
  * to a poor ignorant physicist like myself.  I quote:
  *
  *   The y's covariance matrix T is Toeplitz with diagonals 100,99,...2,1
  *   A Cholesky factorization of T: V'V = T provides M=V^(-1).  The
  *   covariance of x = yM is M'TM=I.
  *
  *   The columns of M have at most 3 non-zero elements.
  *
  * Ouch!  Toeplitz!  Cholesky factorization!  Off to wikipedia or
  * the wolfram site to figure out what that means in plain English...;-)
  *
  * Nilpotent Markov Chains... (by A.M. Alhakim, J. Kawczak and S.
  * Molchanov) is no better, although it certainly appears to be more
  * mathematically valid and consistent.  I may have to work with a
  * "real statistician" on this one to get a scalable code framework
  * established, ideally one where the "100" used in sums is a parameter.
  *
  * As it is now, by the way.  Generators that fail when the number of
  * test samples is only 100 PASS when I crank up the number of samples
  * to around 200 or 300, then fail consistently with a distribution of
  * p that is now biased LOW (where before it was biased HIGH) for all
  * the "best" rngs in the GSL.  Again, this suggests that there is a
  * very subtle error somewhere but is far from definitive.  If you are
  * a real stats person and a coder and are reading this, PLEASE consider
  * helping out by figuring this out and contributing a fix -- otherwise
  * I'll fix it (or at least address it) when I have time to get to it.
  */

 /*
  * Initialize and zero x (used in both overlapping and non-overlapping
  * sums below).
  */
 x = (double *)malloc(test[0]->tsamples*sizeof(double));
 memset(x,0,test[0]->tsamples*sizeof(double));

 /*
  * Initialize y.  We're going to play a curious little pointer trick
  * with the y-sums.  The following creates y[-1] to y[test[0]->tsamples-1],
  * inclusive and zeros them all.
  */
 y = (double *)malloc((test[0]->tsamples+1)*sizeof(double));
 memset(y,0,(test[0]->tsamples+1)*sizeof(double));
 y = &y[1];

 /*
  * Fill a ring of rands of length m, summing them into y[-1] at the
  * same time.  This will seed the t loop below nicely.
  */
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("#==================================================================\n");
   printf("# Initializing initial y[-1] and ring of rands\n");
 }
 for(t=0;t<test[0]->tsamples;t++){
   diehard_sums_rand_dbl[t] = gsl_rng_uniform(rng);
   y[-1] += diehard_sums_rand_dbl[t];
   if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
     printf("y[-1] =  %f\n",y[-1]);
   }
 }
   
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("#==================================================================\n");
   printf("# Now we generate the set of %u overlapping y's\n",test[0]->tsamples);
 }
 for(t=0;t<test[0]->tsamples;t++){
   /*
    * This index points into the ring of rands and
    * circulates through 0-(m-1) as t increments.
    */
   /* Subtract the rand at the ring pointer */
   y[t] = y[t-1] - diehard_sums_rand_dbl[t];
   /* Get a new rand into the ring at the pointer */
   diehard_sums_rand_dbl[t] = gsl_rng_uniform(rng);
   /* Add the new rand at the ring pointer */
   y[t] += diehard_sums_rand_dbl[t];
   if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
     printf("y[%u] =  %f\n",t,y[t]);
   }
 }

 /*
  * Now, for a variety of reasons we need to scale this so
  * that it has mean 0 and unit deviation.
  */
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("#==================================================================\n");
   printf("# We shift and scale y so it should have mean 0.0\n");
 }
 for(t=0;t<test[0]->tsamples;t++){
   y[t] = (y[t] - 0.5*test[0]->tsamples)*sqrt(12.0);
   if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
     printf("y[%u] =  %f\n",t,y[t]);
   }
 }

 /*
  * We convert these (evaluated either way) to a unit normal and thence
  * to a uniform distribution.
  */
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("#==================================================================\n");
   printf("# We convert it to a normal distribution of width 1.0\n");
 }
 x[0] = y[0]/sqrt((double)test[0]->tsamples);
 x[1] = -x[0]*(test[0]->tsamples-1)/sqrt(2.0*m - 1.0)
             +y[1]*sqrt(test[0]->tsamples/(2.0*m - 1.0));
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("x[0] = %f\n",x[0]);
   printf("x[1] = %f\n",x[1]);
 }

 for(t=2;t<test[0]->tsamples;t++){
   a = 2.0*m + 1.0 - t;
   b = 2.0*a - 2.0;
   x[t] = y[0]/sqrt(a*b)- sqrt((a-1.0)/(b+2.0))*y[t-1] + sqrt(a/b)*y[t];
   if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
     printf("x[%u] = %f\n",t,x[t]);
   }
 }

 /*
  * Finally, we convert it into a uniform distribution by making
  * it into an Xtest, as it were.
  */
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   printf("#==================================================================\n");
   printf("# Finally, we turn it into a vector of presumably uniform deviates.\n");
 }
 for(t=0;t<test[0]->tsamples;t++){
   ptest.x = x[t];
   ptest.y = 0.0;
   ptest.sigma = 1.0;
   Xtest_eval(&ptest);
   x[t] = ptest.pvalue;
   /* Test this little (should be unnecessary) chunk:
   printf("Before correction: x[%u] = %f  h = %f  f[%d] = %f\n",t,x[t],h,j,f[j]);
   h = 100.0*ptest.pvalue;
   j = h;
   x[t] = f[j] + (h - (double) j)*(f[t+1] - f[t]);
   */
   if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
     printf("After correction: x[%u] = %f \n",t,x[t]);
   }
 }
 if(verbose == D_DIEHARD_SUMS || verbose == D_ALL){
   histogram(x,test[0]->tsamples,0.0,1.0,10,"x-values");
 }

 /*
  * At last, the x[i]'s are a presumably UNIFORM distribution from 0-1, in
  * fact a distribution of test[0]->tsamples p-values!  We can then do the same old
  * ks test on them that we'll eventually do on ks_pvalue[]!
 test[0]->pvalues[irun] = kstest_kuiper(x,test[0]->tsamples);
  */
 test[0]->pvalues[irun] = kstest(x,test[0]->tsamples);
 MYDEBUG(D_DIEHARD_SUMS) {
   printf("# diehard_sums(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 free(x);
 free(&y[-1]);

}

