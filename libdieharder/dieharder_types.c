/*
 * This is a hack of the GSL's rng/types.c:
 * 
 * Copyright (C) 2001 Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *******************************************************************
 * This is being directly adapted from and modified for use in dieharder
 * so it can maintain its own independent RNG space of types not limited
 * by its internal limit of 100.  To avoid collisions, we'll start our number
 * space above 100, and extend it to 1000.
 *
 * While we're at it, let's define the ranges:
 *
 *   0-99 gsl generators (fixed order from now on)
 *   100-199 reserved for spillover if the gsl ends up with >100 generators
 *   200-399 libdieharder generators (fixed order from now on)
 *   400-499 R-based generators (fixed order from now on)
 *   500-799 reserved for future integration with R-like environments
 *   800-899 user-defined generators (starting with dieharder example)
 *   900-999 hardware generators (starting with /dev/random and friends)
 *
 * Naturally, we can simply bump N below and add new ranges as needed, but
 * these will suffice for now.
 */

#include <dieharder/libdieharder.h>
FILE *test_fp;

const gsl_rng_type **dieharder_rng_types_setup(void)
{

  int i;

  /*
   * Null the whole thing for starters
   */
  for(i=0;i<MAXRNGS;i++) dieharder_types[i] = 0;

  /*
   * Initialize gsl_types to fill it with the current gsl rngs.
   */
  gsl_types = gsl_rng_types_setup();

  /*
   * Copy its contents over into dieharder_rng_generator_types.
   */
  i = 0;
  while(gsl_types[i] != NULL){
    dieharder_types[i] = gsl_types[i];
    i++;
  }

  /*
   * Now add the new ones in.  These positions are to be locked in by
   * order within the ranges, so we need to be careful to get them
   * "right" the first time.
   *
   * These are the dieharder generators.  I expect many users to use
   * stdin-based raw input since it is by far the easiest one to come up
   * with (and actually will work with e.g. /dev/random).  The file-based
   * inputs will also be fairly common.  The rest are there for convenience,
   * and to expose users to some new/interesting rngs.
   */
  i = 200;
  ADD(gsl_rng_stdin_input_raw);
  ADD(gsl_rng_file_input_raw);
  ADD(gsl_rng_file_input);
  ADD(gsl_rng_ca);
  ADD(gsl_rng_uvag);

  /*
   * These are the R-based generators.  Honestly it would be lovely
   * to merge them with the GSL permanently.
   */
  i = 400;
  ADD(gsl_rng_r_wichmann_hill);
  ADD(gsl_rng_r_marsaglia_mc);
  ADD(gsl_rng_r_super_duper);
  ADD(gsl_rng_r_mersenne_twister);
  ADD(gsl_rng_r_knuth_taocp);
  ADD(gsl_rng_r_knuth_taocp2);

  /*
   * These are hardware/system generators.  Again, it would be lovely to
   * merge them with the GSL permanently.  It would also be good to wrap
   * these in conditionals so that they are added iff the hardware
   * interface exists.  Perhaps we should try doing this -- it requires a
   * call to stat, I believe.  But not now.
   */
  i = 500;
  if (test_fp = fopen("/dev/random","r")) {
    ADD(gsl_rng_dev_random);
    fclose(test_fp);
  }
  if (test_fp = fopen("/dev/urandom","r")) {
    ADD(gsl_rng_dev_urandom);
    fclose(test_fp);
  }
  if (test_fp = fopen("/dev/arandom","r")) {
    ADD(gsl_rng_dev_arandom);
    fclose(test_fp);
  }

  /*
   * This will let me change a single word in startup.c in dieharder
   * and the program should "just work" with my custom generators and
   * types in their new places.  We'll see, of course...
   */
  return dieharder_types;

}

