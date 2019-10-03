/*
 *========================================================================
 * $Id: startup.c 324 2007-05-21 23:46:44Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This routine does all the required initialization and startup,
 * including memory allocation and prefilling of vectors.  It is
 * COMPLETELY outside the timing loops.
 *========================================================================
 */

#include "dieharder.h"

void startup()
{

 int i,imax,j,k;
 uint mask;
 struct stat sbuf;


 /*
  * The very first thing we do is see if any simple help options
  * were requested, as we exit immediately if they were and can
  * save all sorts of work.
  */
 if(list == YES) {

printf("\n\
                     DieHarder Test Suite\n\
========================================================================\n\
The following tests are available and will be run when diehard -a is\n\
invoked.  Special options or suggested parameters are indicated if\n\
they are needed to get a satisfactory result (such as completion in a\n\
reasonable amount of time).\n\
\n\
            Diehard Tests\n\
   -d 1  Diehard Birthdays test\n\
   -d 2  Diehard Overlapping Permutations test\n\
   -d 3  Diehard 32x32 Binary Rank test\n\
   -d 4  Diehard 6x8 Binary Rank test\n\
   -d 5  Diehard Bitstream test\n\
   -d 6  Diehard OPSO test\n\
   -d 7  Diehard OQSO test\n\
   -d 8  Diehard DNA test\n\
   -d 9  Diehard Count the 1s (stream) test\n\
   -d 10 Diehard Count the 1s (byte) test\n\
   -d 11 Diehard Parking Lot test\n\
   -d 12 Diehard Minimum Distance (2D Spheres) test\n\
   -d 13 Diehard 3D Spheres (minimum distance) test\n\
   -d 14 Diehard Squeeze test\n\
   -d 15 Diehard Sums test\n\
   -d 16 Diehard Runs test\n\
   -d 17 Diehard Craps test\n\
   -d 18 Marsaglia and Tsang GCD test\n\
   -d 19 Marsaglia and Tsang Gorilla test\n\
\n\
             RGB Tests\n\
   -r 1 Timing test (times the rng)\n\
   -r 2 Bit Persist test\n\
   -r 3 Bit Ntuple Distribution test suite (-n ntuple)\n\
   -r 4 L-M-Ntuple Distribution test suite (quite long)\n\
\n\
      Statistical Test Suite (STS)\n\
   -s 1 STS Monobit test\n\
   -s 2 STS Runs test\n\
\n\
            User Tests\n\
   -u 1 User Template (Lagged Sum Test)\n\
\n");

   Exit(0);
 }

 /*
  * Allocate fields early in case we need to parse()
 fields = allocate_fields(MAXFIELDNUMBER,K);
  */

 /*
  * Count and optionally list the available, built in gsl generators
  */
 types = gsl_rng_types_setup ();
 i = 0;
 while(types[i] != NULL){
   i++;
 }
 num_gsl_rngs = i;

 /*
  * Now add my own types and count THEM.
  */
 add_my_types();
 while(types[i] != NULL){
   i++;
 }

 num_rngs = i;
 num_my_rngs = num_rngs - num_gsl_rngs;

 if(generator == -1){
   list_rngs();
   Exit(0);
 }

 if(generator > num_rngs-1){
   fprintf(stderr,"Error:  rng %d (> %d) does not exist!\n",generator,num_rngs-1);
   list_rngs();
   Exit(0);
 }

 /*
  * We need a sanity check for file input.  File input is permitted
  * iff we have a file name, if the output flag is not set, AND if
  * generator is either file_input or file_input_raw.  Otherwise
  * IF generator is a file input type (primary condition) we punt
  * with a list of types and a honk.
  */
 if(strncmp("file_input",types[generator]->name,10) == 0){
   if(fromfile != 1){
     fprintf(stderr,"Error: generator %s uses file input but no file has been loaded",types[generator]->name);
     list_rngs();
     Exit(0);
   }
   if(output){
     fprintf(stderr,"Error: generator %s uses file input but output flag set.",types[generator]->name);
     Usage();
     Exit(0);
   }
 }

 /*
  * If we get here, in principle the generator is valid and the right
  * inputs are defined to run it (in the case of file_input). We therefore
  * initialize the selected gsl rng using (if possible) random_seed()
  * seeds from /dev/random.  Note that we had to wait until now for the to
  * do this or we'd miss our own additions!
  */
 if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
   fprintf(stdout,"# startup(): Creating and seeding generator %s\n",types[generator]->name);
 }
 rng = gsl_rng_alloc (types[generator]);
 random_max = gsl_rng_max(rng);
 if(Seed == 0){
   seed = random_seed();
   if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
     fprintf(stdout,"# startup(): Generating random seed %u\n",seed);
   }
 } else {
   seed = Seed;
   if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
     fprintf(stdout,"# startup(): Setting random seed %u by hand.\n",seed);
   }
 }
 gsl_rng_set(rng,seed);

 if(output){
   output_rnds();
   /* We'll fix it so we don't have to exit here later, maybe. */
   Exit(0);
 }

 /*
  * Simultaneously count the number of significant bits in the rng
  * AND create a mask (which we need in e.g. rgb_persist and possibly
  * elsewhere).
  */
 rmax = random_max;
 rmax_bits = 0;
 rmax_mask = 0;
 while(rmax){
   rmax >>= 1;
   rmax_mask = rmax_mask << 1;
   rmax_mask++;
   rmax_bits++;
 }

 /*
  * TESTING PLAYGROUND.  bits.c code MUST be carefully validated as it is
  * quite complex and crucial to happy testing.  We cannot test this until
  * AFTER rng is set up.
  testbits();
  Exit(0);
  *
  */

 /*
  * Allocate the global vector that will hold random integers
  * loaded from gsl generators (usually) or from a file (sometimes).
  * One day (soon) we'll need to figure out file I/O.  Not exactly
  * trivial, as many of the tests determine how many rands they
  * need depending strongly on the test parameters -- to be able
  * to run arbitrary tests, the file will need to be quite large.
  * We may need to write a routine that fills rand_int on demand
  * from either the file or a generator.
  *
  * This size is possibly overkill, but otherwise we'd have to
  * figure out how big it is per test.  This is big enough
  * for all of them, I'm pretty sure.
  *
  * Actually, I'm hoping that this is cruft, but it probably isn't,
  * yet.  We do need to alter how it is used, though, and ALWAYS
  * allocate/free it on a per-test basis...
 rand_int = (uint *) malloc((size_t) (tsamples*sizeof(unsigned int)));
  */

 /*
  * This is the global vector of p-values generated by each test
  * run many times.  It has to be oversized because resizing it
  * to just right is more of a hassle then just spending the memory
  * in an era where systems with less than a GB of active memory
  * will be increasingly rare.  These p-values are ONLY used in
  * the end-stage e.g. KS tests that globally validate the distribution
  * of p-values returned by the test.  Set the kspi index to point to
  * the first element of the vector.
  *
  * I think the following is cruft
 ks_pvalue = (double *)malloc((size_t) KS_SAMPLES_PER_TEST_MAX*psamples*sizeof(double));
 ks_pvalue2 = (double *)malloc((size_t) KS_SAMPLES_PER_TEST_MAX*psamples*sizeof(double));
  *
  * and I'm HOPING that every test uses an absolute maximum of psamples
  * samples for its concluding KS test, although in the case of e.g. diehard
  * tests this is probably not always true.  I actually think that the
  * right way to do this at this point is to malloc the memory inside
  * the test itself and free it at the end.  Why not?
  */
 ks_pvalue = 0;
 ks_pvalue2 = 0;
 kspi = 0;

}
