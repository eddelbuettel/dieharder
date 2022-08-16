/*
 * diehard_predict_mersenne test header.
 */

/*
 * function prototype
 */
int diehard_predict_mersenne(Test **test, int irun);

static Dtest diehard_predict_mersenne_dtest __attribute__((unused)) = {
  "Diehard Predict Mersenne Test",
  "diehard_predict_mersenne",
  "Predict future output from the Mersenne Twister RNG",
  1,
  100,
  1,
  diehard_predict_mersenne,
  0
};

/*
 * Global variables
uint diehard_predict_mersenne_nms,diehard_predict_mersenne_nbits;
uint *diehard_predict_mersenne_rand_uint;
 */
