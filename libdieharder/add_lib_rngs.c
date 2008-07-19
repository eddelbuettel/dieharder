/*
 *========================================================================
 * $Id: add_lib_rngs.c 331 2007-06-08 14:14:04Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include <dieharder/libdieharder.h>

void add_lib_rngs()
{

 int i = 0;

 /*
  * Get to the end of types
  */
 while(types[i] != NULL){
   i++;
 }

 /*
  * and add the new ones...
  */
 types[i] = (gsl_rng_dev_random);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_dev_urandom);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_file_input);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_file_input_raw);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_ca);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_wichmann_hill);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_marsaglia_mc);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_super_duper);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_mersenne_twister);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_knuth_taocp);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_r_knuth_taocp2);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 types[i] = (gsl_rng_uvag);
 if(verbose) printf("# add_my_types():  Added type %d = %s\n",i,types[i]->name);
 i++;
 
}
