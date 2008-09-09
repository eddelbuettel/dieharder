/*
 *========================================================================
 * $Id: dieharder.c 127 2004-11-20 18:17:55Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 *  run_rgb_timing()
 *========================================================================
 */

#include "dieharder.h"

void run_user_template()
{


 /*
  * Declare the results struct.
  */
 Test **user_template_test;

 /*
  * First we create the test (to set some values displayed in test header
  * correctly).
  */
 user_template_test = create_test(&user_template_dtest,tsamples,psamples,&user_template);
 user_template_test[0]->ntuple = ntuple;

 /*
  * Set any GLOBAL data used by the test.  Then call the test itself
  * This fills in the results in the Test struct.
  */
 std_test(&user_template_dtest,user_template_test);

 /*
  * Output test results.
  */
 output(&user_template_dtest,user_template_test);

 /*
  * Destroy the test and free all dynamic memory it used.
  */
 destroy_test(&user_template_dtest,user_template_test);

}
