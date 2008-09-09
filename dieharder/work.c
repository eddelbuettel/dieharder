/*
 *========================================================================
 * $Id: work.c 448 2008-09-07 17:58:00Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This should be a nice, big case switch where we add EACH test
 * we might want to do and either just configure and do it or
 * prompt for input (if absolutely necessary) and then do it.
 *========================================================================
 */

#include "dieharder.h"

void work()
{

 if(output_file == YES){
   output_rnds();
 }

 /*
  * Let us simply "always" check the timing of an rng -- it doesn't take
  * and it seems like it should be part of a standard report regardless.
  * Dirk Eddelbuettel:  Let's not do this, it breaks sequential use
 if(!fromfile){ 
   run_rgb_timing();
 }
  */

 if(table){
   table_header();
 }

 if(all == YES){
   /* run_rgb_persist(); */
   run_rgb_bitdist();
   run_rgb_minimum_distance();
   run_rgb_permutations();
   run_rgb_lagged_sums();
   /*  run_rgb_operm(); */
   run_diehard_birthdays();
   /* run_diehard_operm5(); */
   run_diehard_rank_32x32();
   run_diehard_rank_6x8();
   run_diehard_bitstream();
   run_diehard_opso();
   run_diehard_oqso();
   run_diehard_dna();
   run_diehard_count_1s_stream();
   run_diehard_count_1s_byte();
   run_diehard_parking_lot();
   run_diehard_2dsphere();
   run_diehard_3dsphere();
   run_diehard_squeeze();
   /* run_diehard_sums(); */
   run_diehard_runs();
   run_diehard_craps();
   run_marsaglia_tsang_gcd();
   /* marsaglia_tsang_gorilla(); */
   run_sts_monobit();
   run_sts_runs();
   run_sts_serial();
   run_user_template();
   Exit(0);
 }

 switch(diehard){
   case DIEHARD_BDAY:
     run_diehard_birthdays();
     break;
   case DIEHARD_OPERM5:
     run_diehard_operm5();
     break;
   case DIEHARD_RANK_32x32:
     run_diehard_rank_32x32();
     break;
   case DIEHARD_RANK_6x8:
     run_diehard_rank_6x8();
     break;
   case DIEHARD_BITSTREAM:
     run_diehard_bitstream();
     break;
   case DIEHARD_OPSO:
     run_diehard_opso();
     break;
   case DIEHARD_OQSO:
     run_diehard_oqso();
     break;
   case DIEHARD_DNA:
     run_diehard_dna();
     break;
   case DIEHARD_COUNT_1S_STREAM:
     run_diehard_count_1s_stream();
     break;
   case DIEHARD_COUNT_1S_BYTE:
     run_diehard_count_1s_byte();
     break;
   case DIEHARD_PARKING_LOT:
     run_diehard_parking_lot();
     break;
   case DIEHARD_2DSPHERE:
     run_diehard_2dsphere();
     break;
   case DIEHARD_3DSPHERE:
     run_diehard_3dsphere();
     break;
   case DIEHARD_SQUEEZE:
     run_diehard_squeeze();
     break;
   case DIEHARD_SUMS:
     run_diehard_sums();
     break;
   case DIEHARD_RUNS:
     run_diehard_runs();
     break;
   case DIEHARD_CRAPS:
     run_diehard_craps();
     break;
   case MARSAGLIA_TSANG_GCD:
     run_marsaglia_tsang_gcd();
     break;
   case MARSAGLIA_TSANG_GORILLA:
     /* marsaglia_tsang_gorilla(); */
     break;
   default:
     break;
 }

 switch(rgb){
   case RGB_TIMING:
     run_rgb_timing();
     break;
   case RGB_PERSIST:
     run_rgb_persist();
     break;
   case RGB_BITDIST:
     run_rgb_bitdist();
     break;
   case RGB_MINIMUM_DISTANCE:
     run_rgb_minimum_distance();
     break;
   case RGB_PERMUTATIONS:
     run_rgb_permutations();
     break;
   case RGB_LAGGED_SUMS:
     run_rgb_lagged_sums();
     break;
   case RGB_LMN:
     rgb_lmn();
     break;
   case RGB_OPERM:
     run_rgb_operm();
     break;
   default:
     break;
 }

 switch(sts){
   case STS_MONOBIT:
     run_sts_monobit();
     break;
   case STS_RUNS:
     run_sts_runs();
     break;
   case STS_SERIAL:
     run_sts_serial();
     break;
   default:
     break;
 }

 switch(user){
   case USER_TEMPLATE:
     run_user_template();
     break;
   default:
     break;
 }

}
