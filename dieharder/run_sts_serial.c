/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_rgb_bitdist()
 *========================================================================
 */

#include "dieharder.h"

void run_sts_serial()
{

 /*
  * Declare the results struct.
  */
 Test **sts_serial_test;
 int i;

 /*
  * Set any GLOBAL data used by the test.  We will gradually
  * adjust the testing struct and sequence as needed to permit
  * a whole set of tests to be run with a single call, and be
  * stored correctly in a vector of pvalues EACH WITH THEIR
  * OWN DESCRIPTION LINE that can be set in the testing routine.
  *
  * So we actually will not use or need any global ntuple variable
  * here -- we only run the actual sts_serial test ONCE.
  */

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 sts_serial_test = create_test(&sts_serial_dtest,tsamples,psamples,&sts_serial);

 /*
  * This particular test we need to pre-initialize the pvlabel for
  * each test, in order.
  */
 snprintf(sts_serial_test[0]->pvlabel,LINE,"# Normal p-value for STS Serial test for n=1 bit (STS Monobit)\n");
 snprintf(sts_serial_test[1]->pvlabel,LINE,"# p-value 1 for STS Serial test for n=2 bits\n");
 for(i=0;i<14;i++){
    snprintf(sts_serial_test[2*i+2]->pvlabel,LINE,"# p-value 1 for STS Serial test for n=%u bits\n",i+3);
    snprintf(sts_serial_test[2*i+3]->pvlabel,LINE,"# p-value 2 for STS Serial test for n=%u bits\n",i+3);
 }
   
 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&sts_serial_dtest,sts_serial_test);

 /*
  * Output standard test results.
  */
 output(&sts_serial_dtest,sts_serial_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&sts_serial_dtest,sts_serial_test);

}
