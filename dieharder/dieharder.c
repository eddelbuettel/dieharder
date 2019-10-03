/*
 *========================================================================
 * $Id: dieharder.c 438 2008-09-05 11:16:01Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"


/*
 * main() is just a shell for routines that parse the command line,
 * do all the requested work, and then exit cleanly.
 */
int main(int argc, char *argv[]) 
{

 int i;

 /*
  * Parse command line and set global variables
  */
 parsecl(argc,argv);

 /*
  * Startup: Allocate memory, initialize all derivative variables from
  * command line values.  
  */
 /* for(i=0;i<=2;i++){ */
 /* printf("Running startup for i = %d\n",i); */
 /* printf("Seed = %u, seed = %u\n",Seed,seed); */
 startup();

 /*
  * Work: Do all the work.
  */
 /* printf("Running work for i = %d\n",i); */
 work();

 /*
  * Clean up:  If we're going to run inside a loop, we MUST free the
  * generator allocated in startup() and reset the bit buffers used in
  * bits.c or we won't get consistent results run to run and might leak
  * memory slowly.
  */

 /* printf("Freeing generator %s and moving on.\n",gsl_rng_name(rng)); */
 gsl_rng_free(rng);

 /* printf("Clearing/resetting bit buffers.\n"); */
 reset_bit_buffers();

 /* } */

}
