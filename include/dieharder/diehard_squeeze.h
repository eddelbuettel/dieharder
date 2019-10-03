/*
 * diehard_squeeze test header.
 */

/*
 * function prototype
 */
void diehard_squeeze(Test **test,int irun);

static Dtest diehard_squeeze_dtest = {
  "Diehard Squeeze Test",
  "diehard_squeeze",
  "\
#==================================================================\n\
#                  Diehard Squeeze Test.\n\
#  Random integers are floated to get uniforms on [0,1). Start- \n\
#  ing with k=2^31=2147483647, the test finds j, the number of  \n\
#  iterations necessary to reduce k to 1, using the reduction   \n\
#  k=ceiling(k*U), with U provided by floating integers from    \n\
#  the file being tested.  Such j's are found 100,000 times,    \n\
#  then counts for the number of times j was <=6,7,...,47,>=48  \n\
#  are used to provide a chi-square test for cell frequencies.  \n\
#==================================================================\n",
  100,
  100000,
  1
};

static double sdata[]={
0.00002103, 0.00005779, 0.00017554, 0.00046732, 0.00110783,
0.00236784, 0.00460944, 0.00824116, 0.01362781, 0.02096849,
0.03017612, 0.04080197, 0.05204203, 0.06283828, 0.07205637,
0.07869451, 0.08206755, 0.08191935, 0.07844008, 0.07219412,
0.06398679, 0.05470931, 0.04519852, 0.03613661, 0.02800028,
0.02105567, 0.01538652, 0.01094020, 0.00757796, 0.00511956,
0.00337726, 0.00217787, 0.00137439, 0.00084970, 0.00051518,
0.00030666, 0.00017939, 0.00010324, 0.00005851, 0.00003269,
0.00001803, 0.00000982, 0.00001121
};
