/*
 *========================================================================
 * $Id: work.c 324 2007-05-21 23:46:44Z rgb $
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

/*
 if(output == YES){
   output_rnds();
 }
 */

 if(all == YES){
   run_rgb_timing();
   run_rgb_persist();
   run_rgb_bitdist();
   run_diehard_birthdays();
   run_diehard_operm5();
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
   run_diehard_sums();
   run_diehard_runs();
   run_diehard_craps();
   run_marsaglia_tsang_gcd();
   /* marsaglia_tsang_gorilla(); */
   run_sts_monobit();
   run_sts_runs();
   run_user_template();
   Exit(0);
 }

 switch(diehard){
   case DIEHARD_BDAY:
     run_diehard_birthdays();
     Exit(0);
     break;
   case DIEHARD_OPERM5:
     run_diehard_operm5();
     Exit(0);
     break;
   case DIEHARD_RANK_32x32:
     run_diehard_rank_32x32();
     Exit(0);
     break;
   case DIEHARD_RANK_6x8:
     run_diehard_rank_6x8();
     Exit(0);
     break;
   case DIEHARD_BITSTREAM:
     run_diehard_bitstream();
     Exit(0);
     break;
   case DIEHARD_OPSO:
     run_diehard_opso();
     Exit(0);
     break;
   case DIEHARD_OQSO:
     run_diehard_oqso();
     Exit(0);
     break;
   case DIEHARD_DNA:
     run_diehard_dna();
     Exit(0);
     break;
   case DIEHARD_COUNT_1S_STREAM:
     run_diehard_count_1s_stream();
     Exit(0);
     break;
   case DIEHARD_COUNT_1S_BYTE:
     run_diehard_count_1s_byte();
     Exit(0);
     break;
   case DIEHARD_PARKING_LOT:
     run_diehard_parking_lot();
     Exit(0);
     break;
   case DIEHARD_2DSPHERE:
     run_diehard_2dsphere();
     Exit(0);
     break;
   case DIEHARD_3DSPHERE:
     run_diehard_3dsphere();
     Exit(0);
     break;
   case DIEHARD_SQUEEZE:
     run_diehard_squeeze();
     Exit(0);
     break;
   case DIEHARD_SUMS:
     run_diehard_sums();
     Exit(0);
     break;
   case DIEHARD_RUNS:
     run_diehard_runs();
     Exit(0);
     break;
   case DIEHARD_CRAPS:
     run_diehard_craps();
     Exit(0);
     break;
   case MARSAGLIA_TSANG_GCD:
     run_marsaglia_tsang_gcd();
     Exit(0);
     break;
   case MARSAGLIA_TSANG_GORILLA:
     /* marsaglia_tsang_gorilla(); */
     Exit(0);
     break;
   default:
     break;
 }

 switch(rgb){
   case RGB_TIMING:
     run_rgb_timing();
     Exit(0);
     break;
   case RGB_PERSIST:
     run_rgb_persist();
     Exit(0);
     break;
   case RGB_BITDIST:
     run_rgb_bitdist();
     Exit(0);
     break;
/*   case RGB_LMN:
     rgb_lmn();
     Exit(0);
     break; */
   default:
     break;
 }

 switch(sts){
   case STS_MONOBIT:
     run_sts_monobit();
     Exit(0);
     break;
   case STS_RUNS:
     run_sts_runs();
     Exit(0);
     break;
   default:
     break;
 }

 switch(user){
   case USER_TEMPLATE:
     run_user_template();
     Exit(0);
     break;
   default:
     break;
 }

#if !defined(RDIEHARDER)
 list_rngs();
#endif   /* !defined(RDIEHARDER */

}
