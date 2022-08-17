/*
 * diehard_predict_mersenne test header.
 */

extern int diehard_predict_mersenne(Test **test, int irun);  // defined in diehard_predict_mersenne.c

static Dtest diehard_predict_mersenne_dtest __attribute__((unused)) = {
  /* The name of the test */ "Diehard Predict Mersenne Test",
  /* The call name        */ "diehard_predict_mersenne",
  /* Text description     */ "Predict future output from the 32-Bit MT19937 Mersenne Twister RNG",
  /* default psamples     */ 1u,
  /* default tsamples     */ 1248u, // This is 2 * 624
  /* Independent statistics generated per run */ 1u,
  /* pointer to function  */ diehard_predict_mersenne,
  /* pointer to a vector of additional test arguments */ 0
};
