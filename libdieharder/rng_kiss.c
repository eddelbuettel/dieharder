/* dev_random
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 James Theiler, Brian Gough
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <dieharder/libdieharder.h>

/*
 * This is a wrapping of the KISS generator
 */

static unsigned long int kiss_get (void *vstate);
static double kiss_get_double (void *vstate);
static void kiss_set (void *vstate, unsigned long int s);

/*
 * perhaps these should be in kiss_state.
 */
static uint kiss_b[4096],x,y,z,w,carry;

typedef struct
  {
  }
kiss_state_t;

static unsigned long int
kiss_get (void *vstate)
{


}

static double
kiss_get_double (void *vstate)
{
  return 0.0;
}

static void
kiss_set (void *vstate, unsigned long int s)
{

 /*
  * kiss needs four random number seeds.  They have to be reproducible
  * from a single seed in order to be consistent with the GSL.  We
  * therefore have to do a two step process where we use seed to
  * seed an existing GSL generator (say mt19937_1999) and take the
  * first three returns as the rest of our seed for this generator.
  */
 
 return;

}

static const gsl_rng_type kiss_type =
{"empty",			/* name */
 0,				/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (kiss_state_t),
 &kiss_set,
 &kiss_get,
 &kiss_get_double};

const gsl_rng_type *gsl_rng_kiss = &kiss_type;
