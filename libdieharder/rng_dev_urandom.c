/* dev_urandom
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
 * This is a wrapping of the /dev/random hardware rng
 */

static unsigned long int dev_urandom_get (void *vstate);
static double dev_urandom_get_double (void *vstate);
static void dev_urandom_set (void *vstate, unsigned long int s);

typedef struct
  {
    FILE *fp;
  }
dev_urandom_state_t;

static unsigned long int
dev_urandom_get (void *vstate)
{
  dev_urandom_state_t *state = (dev_urandom_state_t *) vstate;
  unsigned long int j;

  if(state->fp != NULL) {
    fread(&j,sizeof(j),1,state->fp);
    return j;
  } else {
    fprintf(stderr,"Error: /dev/urandom not open.  Exiting.\n");
    exit(0);
  }

}

static double
dev_urandom_get_double (void *vstate)
{
  return dev_urandom_get (vstate) / (double) UINT_MAX;
}

static void
dev_urandom_set (void *vstate, unsigned long int s)
{
  dev_urandom_state_t *state = (dev_urandom_state_t *) vstate;

 if ((state->fp = fopen("/dev/urandom","r")) == NULL) {
   fprintf(stderr,"Error: Cannot open /dev/urandom, exiting.\n");
   exit(0);
 }

 return;

}

static const gsl_rng_type dev_urandom_type =
{"/dev/urandom",		/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (dev_urandom_state_t),
 &dev_urandom_set,
 &dev_urandom_get,
 &dev_urandom_get_double};

const gsl_rng_type *gsl_rng_dev_urandom = &dev_urandom_type;
