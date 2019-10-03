/*
* $Id: list_rngs.c 420 2008-08-18 18:29:17Z rgb $
*
* See copyright in copyright.h and the accompanying file COPYING
* See also accompanying file STS.COPYING
*
*/

/*
 *========================================================================
 * This just lists the available rng's.  This is the default action of
 * dieharder when no test is specified.
 *========================================================================
 */

#include "dieharder.h"

void list_rngs()
{
 int i,j;

 if(verbose){
   printf("list_rngs():\n");
 }
 /* Version string seems like a good idea */
 printf("==========================================================================|\n");
 printf("         dieharder version %s Copyright 2003 Robert G. Brown         |\n",QUOTEME(VERSION));
 printf("==========================================================================|\n");
 printf("%3s %-20s|%3s %-20s|%3s %-20s|\n"," Id","Test Name",
         " Id","Test Name"," Id","Test Name");
 printf("==========================================================================|\n");
 i = 0;
 j = 0;
 while(types[i] != NULL){
   printf("%3d %-20s|", i, types[i]->name);
   if(((j+1)%3) == 0 && i>0) printf("\n");
   i++;
   j++;
 }
 /*
  * Finish off each block neatly.  If j == 0, we finished the row
  * and do nothing.  Otherwise, pad to the end of the row.
  */
 j = j%3;
 if(j == 1) printf("                        |                        |\n");
 if(j == 2) printf("                        |\n");
 printf("==========================================================================|\n");
 i = 200;
 j = 0;
 while(types[i] != NULL){
   printf("%3d %-20s|", i, types[i]->name);
   if(((j+1)%3) == 0 && i>200) printf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) printf("                        |                        |\n");
 if(j == 2) printf("                        |\n");
 printf("==========================================================================|\n");
 i = 400;
 j = 0;
 while(types[i] != NULL){
   printf("%3d %-20s|", i, types[i]->name);
   if(((j+1)%3) == 0 && i>400) printf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) printf("                        |                        |\n");
 if(j == 2) printf("                        |\n");
 printf("==========================================================================|\n");
 i = 500;
 j = 0;
 while(types[i] != NULL){
   printf("%3d %-20s|", i, types[i]->name);
   if(((j+1)%3) == 0 && i>500) printf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) printf("                        |                        |\n");
 if(j == 2) printf("                        |\n");
 printf("==========================================================================|\n");
 i = 600;
 j = 0;
 while(types[i] != NULL){
   printf("%3d %-20s|", i, types[i]->name);
   if(((j+1)%3) == 0 && i>600) printf("\n");
   i++;
   j++;
 }
 j = j%3;
 if(j == 1) printf("                        |                        |\n");
 if(j == 2) printf("                        |\n");
 printf("==========================================================================|\n");

}

