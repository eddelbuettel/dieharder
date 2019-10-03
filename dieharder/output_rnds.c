/*
 *
 * See copyright in copyright.h and the accompanying file COPYING
 * See also accompanying file STS.COPYING
 *
 */

/*
 *========================================================================
 * Output a list of random numbers of the selected type into a file.
 * Write a nice informative header, as well, to indicate several things
 * about the list (type, generator, count etc.)
 *========================================================================
 */

#include "dieharder.h"

double output_rnds()
{

 uint i,j;
 FILE *fp;

 /*
  * If Seed is set, use it.  Otherwise reseed from /dev/random
  */
 if(Seed){
   seed = Seed;
   gsl_rng_set(rng,seed);
 } else {
   seed = random_seed();
   gsl_rng_set(rng,seed);
 }

 /*
  * Open the output file.  If no filename is specified, or if
  * filename is "-", use stdout.
  */
 if( (filename[0] == 0) || (strncmp("-",filename,1)==0) ){
   fp = stdout;
 } else {
   if ((fp = fopen(filename,"w")) == NULL) {
     fprintf(stderr,"Error: Cannot open %s, exiting.\n",filename);
     exit(0);
   }
 }

 /*
  * If the binary file flag is set, we must have no header
  * or it will be treated as binary input.  If we're outputting
  * an ASCII list, we MUST have a header as the program is too
  * stupid (still) to count things for itself, although I suppose
  * it could.  I like a human readable header on a human readable
  * file, though, so mote it be.
  */
 if(binary == NO){
   fprintf(fp,"#==================================================================\n");
   fprintf(fp,"# generator %s  seed = %u\n",gsl_rng_name(rng),seed);
   fprintf(fp,"#==================================================================\n");
   fprintf(fp,"type: d\ncount: %i\nnumbit: 32\n",tsamples);
 } else {
   if(verbose && fp != stdout) {
     printf("Ascii values of binary data being written into file %s:\n",filename);
   }
 }
 /*
  * make the samples and output them.
  */
 for(i=0;i<tsamples;i++){
   j = gsl_rng_get(rng);
   if(binary){
     fwrite(&j,sizeof(uint),1,fp);
     if(verbose && fp != stdout) {
       printf("%10u\n",j);
     }
   } else {
     fprintf(fp,"%10u\n",j);
   }
 }

 fclose(fp);

}

