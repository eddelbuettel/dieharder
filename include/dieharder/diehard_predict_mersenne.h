/*
 * diehard_predict_mersenne test header.
 */

extern int diehard_predict_mersenne(Test **test, int irun);  // defined in diehard_predict_mersenne.c

static Dtest diehard_predict_mersenne_dtest __attribute__((unused)) = {
  "Diehard Predict Mersenne Test",
  "diehard_predict_mersenne",
  "Predict future output from the 32-Bit MT19937 Mersenne Twister RNG",
  1,
  100,
  1,
  diehard_predict_mersenne,
  0
};
