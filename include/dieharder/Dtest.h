/*
 *========================================================================
 * $Id: libdieharder.h 221 2006-08-16 22:43:03Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

 typedef struct {
   /* The name of the test */
   char *name;

   /* pointer to a test description */
   char *description;

   /* Standard test default */
   uint psamples_std;

   /* Standard test default */
   uint tsamples_std;

   /* Number of independent statistics generated per run */
   uint nkps;

 } Dtest;

