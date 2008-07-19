/*
 *========================================================================
 * $Id: add_lib_rngs.h 325 2007-05-22 14:04:12Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 * This routine just adds new RNG's onto the GSL types list.
 * Apparently this is all that is needed -- they subsequently
 * just "work" in the gsl call format.  Note that there DOES
 * have to be a subroutine set with its own state and so forth just
 * like the one in dev_random.c (for the linus entropy generator).
 *
 * From gsl types.c -- MAXRNGS apparently a hard-coded value.  This means
 * that we cannot CURRENTLY go over 100 generators without e.g.
 * increasing N in the gsl sources.  They do leave some room for
 * new ones, but not a lot.  Fortunately, I probably won't ever
 * fill N myself personally...
 */

#define MAXRNGS 100

 /* #define GSL_VAR */
 /* List new rng types to be added. */
 GSL_VAR const gsl_rng_type *gsl_rng_dev_random;
 GSL_VAR const gsl_rng_type *gsl_rng_dev_urandom;
 GSL_VAR const gsl_rng_type *gsl_rng_empty_random;
 GSL_VAR const gsl_rng_type *gsl_rng_file_input;
 GSL_VAR const gsl_rng_type *gsl_rng_file_input_raw;
 GSL_VAR const gsl_rng_type *gsl_rng_ca;
 GSL_VAR const gsl_rng_type *gsl_rng_r_wichmann_hill;	/* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_r_marsaglia_mc;	/* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_r_super_duper;	/* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_r_mersenne_twister;  /* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_r_knuth_taocp;	/* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_r_knuth_taocp2;	/* edd May 2007 */
 GSL_VAR const gsl_rng_type *gsl_rng_uvag;	        /* rgb June 2007 */

 /*
  * rng global vectors and variables for setup and tests.
  */
 const gsl_rng_type **types;    /* where all the rng types go */
 gsl_rng *rng;                  /* global gsl random number generator */

