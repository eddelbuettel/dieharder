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

#include "dieharder.h"

/*
 * This is a wrapping of the /dev/random hardware rng
 */

static unsigned long int dev_random_get (void *vstate);
static double dev_random_get_double (void *vstate);
static void dev_random_set (void *vstate, unsigned long int s);

typedef struct
  {
  }
dev_random_state_t;

static unsigned long int
empty_random_get (void *vstate)
{

}

static double
empty_random_get_double (void *vstate)
{
  return 0.0;
}

static void
empty_random_set (void *vstate, unsigned long int s)
{

 return;

}

static const gsl_rng_type empty_random_type =
{"empty",			/* name */
 0,				/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (dev_random_state_t),
 &empty_random_set,
 &empty_random_get,
 &empty_random_get_double};

const gsl_rng_type *gsl_rng_empty_random = &empty_random_type;
