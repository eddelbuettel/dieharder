/*
 *=========================================================
 *        DAB Fill Tree 2 Test
 * Bit version of Fill Tree test.
 * This test fills small binary trees of fixed depth with
 * "visited" markers.  When a marker cannot be placed, the
 * current count of markers in the tree and the position
 * that the marker would have been inserted, if it hadn't
 * already been marked.
 *
 * For each bit in the RNG input, the test takes a step
 * right (for a zero) or left (for a one) in the tree.
 * If the node hasn't been marked, it is marked, and the
 * path restarts.  Otherwise, the test continues with the
 * next bit.
 *
 * The test returns two p-values.  The first is a Pearson
 * chi-sq test against the expected values (which were
 * estimated empirically.  The second is a Pearson chi-sq
 * test for a uniform distribution of the positions at
 * which the insert failed.
 *
 * Because of the target data for the first p-value,
 * ntuple must be kept at the default (128).
 */
#include <dieharder/libdieharder.h>

typedef unsigned char uchar;

static double targetData1[32] __attribute__((unused)) = {  // size=32, exact
0,0,0,0,
0.0009765625,0.00390625,0.009429931640625,0.0177898406982422,
0.0288577079772949,0.0421273708343506,0.0566971004009247,0.0712958071380854,
0.0843847030773759,0.0943488389020786,0.0997659675704199,0.0997059316159721,
0.0939841693480048,0.0832818323431184,0.0690661567389839,0.0533005473930537,
0.0380100025885399,0.0248326931670262,0.0147065833262707,0.00779190452613503,
0.00363217487506055,0.00145753559952102,0.000488833092230661,0.000131328293435103,
2.64480590945694E-05,3.54215077159411E-06,2.36143384772941E-07,0
};

static double targetData2[64] __attribute__((unused)) = {  // size=64, exact
0,0,0,0,
0,3.0517578125E-05,0.000152587890625,0.000447511672973633,
0.00100504606962204,0.00191331189125776,0.00325121614150703,0.00508295510371681,
0.0074537967325341,0.010386477114821,0.01387787701767,0.0178959820102482,
0.0223773962832272,0.0272258597146006,0.0323123111057767,0.037477047033474,
0.0425344401223118,0.0472804976516124,0.0515032630016068,0.054995703372204,
0.0575703200065506,0.0590743134364583,0.059403803645923,0.0585154187223134,
0.0564335947898278,0.0532522211136633,0.0491298234656811,0.0442782596874916,
0.0389458016037146,0.0333963492622822,0.0278871984136495,0.0226481070118066,
0.0178642831703181,0.0136653347694681,0.0101212706649298,0.00724550563200844,
0.00500372727772275,0.00332665901853009,0.00212436000041605,0.00129979403042639,
0.000759908933033483,0.000423232777916195,0.000223807074715186,0.000111949408083441,
5.27471340854235E-05,2.32990903233354E-05,9.59581500880408E-06,3.66187793596757E-06,
1.28535889122115E-06,4.11412264488149E-07,1.18830307817474E-07,3.05774193583426E-08,
6.89718891789191E-09,1.33534096493464E-09,2.15638141871131E-10,2.78699898584813E-11,
2.70138143225745E-12,1.74464217499961E-13,5.62787798386969E-15,0
};

static double targetData[128] = {  // size=128, exact
0,0,0,0,
0,0,4.76837158203125E-07,2.86102294921875E-06,
9.85432416200638E-06,2.55650229519233E-05,5.54892931177164E-05,0.000106380728425393,
0.000186047948883328,0.00030311244166753,0.000466749839810675,0.000686429646575251,
0.000971661742344283,0.00133175306183679,0.00177557452024558,0.00231133638265425,
0.00294636954701581,0.00368691038033341,0.00453788756811422,0.00550271070203945,
0.00658306187040692,0.00777869319248569,0.00908723493764983,0.0105040204946228,
0.0120219359149386,0.0136313029599996,0.0153198054448571,0.0170724691056809,
0.0188717051355292,0.0206974268550898,0.0225272476458152,0.0243367662285437,
0.0260999426084925,0.0277895645547568,0.0293778004147126,0.0308368295109554,
0.0321395365179289,0.0332602513106519,0.0341755111120263,0.0348648176685728,
0.0353113590065781,0.0355026634060874,0.0354311528899465,0.035094565005384,
0.0344962151263554,0.0336450769511817,0.0325556661902256,0.031247721351896,
0.0297456856036487,0.0280780043276814,0.0262762635190492,0.0243742038353947,
0.0224066531487575,0.0204084261828087,0.0184132426894747,0.0164527152702953,
0.0145554542687502,0.0127463303124921,0.0110459255065655,0.00947019265623616,
0.00803032911044744,0.00673285885868451,0.00557990440062551,0.00456961957621112,
0.00369674675854223,0.0029532570905934,0.00232903101043375,0.00181253806897198,
0.00139147961934567,0.00105336473071676,0.00078599787487321,0.000577865708395529,
0.00041841881087046,0.000298251837753283,0.000209191678462234,0.000144307562906168,
9.78595402557837E-05,6.520247003055E-05,4.26618834333199E-05,2.73961611501714E-05,
1.7256844789337E-05,1.06559535882152E-05,6.44626192690798E-06,3.81786954553569E-06,
2.2122334819058E-06,1.25320523087539E-06,6.9352747611892E-07,3.74634041706112E-07,
1.97372359804204E-07,1.01324484479249E-07,5.06387688071287E-08,2.4612770858879E-08,
1.16222051625842E-08,5.32575018016149E-09,2.36548132737708E-09,1.01707372778487E-09,
4.22755189488443E-10,1.69627368106376E-10,6.55983218678784E-11,2.44087465306515E-11,
8.72288816181913E-12,2.98796462656132E-12,9.78930546827611E-13,3.06028628289706E-13,
9.10492627063714E-14,2.57067414678584E-14,6.86573813128305E-15,1.72840983639153E-15,
4.08488586235289E-16,9.02205965579907E-17,1.85248682021962E-17,3.5148157083845E-18,
6.11888831372621E-19,9.69194996799213E-20,1.38262127072485E-20,1.75429503989084E-21,
1.94858024047957E-22,1.85585143679842E-23,1.47329706646149E-24,9.35788996513105E-26,
4.45783630198697E-27,1.41554277734465E-28,2.24689329737247E-30,0
};

static inline int insertBit(uint x, uchar *array, uint *i, uint *d);

int dab_filltree2(Test **test, int irun) {
 int size = (ntuple == 0) ? 128 : ntuple;
 uint target = sizeof(targetData)/sizeof(double);
 int startVal = (size / 2) - 1;
 uchar *array = (uchar *) malloc(sizeof(*array) * size);
 double *counts, *expected;
 int i, j;
 uint x;
 uint start = 0;
 uint end = 0;
 uint *positionCounts;
 uint bitCount;

 test[0]->ntuple = 0;
 test[1]->ntuple = 1;

 counts = (double *) malloc(sizeof(double) * target);
 expected = (double *) malloc(sizeof(double) * target);

 memset(counts, 0, sizeof(double) * target);

 positionCounts = (uint *) malloc(sizeof(uint) * size/2);
 memset(positionCounts, 0, sizeof(uint) * size/2);

 /* Calculate expected counts. */
 for (i = 0; i < target; i++) {
   expected[i] = targetData[i] * test[0]->tsamples;
   if (expected[i] < 4) {
     if (end == 0) start = i;
   } else if (expected[i] > 4) end = i;
 }
 start++;


 x = gsl_rng_get(rng);
 bitCount = rmax_bits;
 for (j = 0; j < test[0]->tsamples; j++) {
   int ret;
   memset(array, 0, sizeof(*array) * size);
   i = 0;
   do {  /* While new markers can be aded to this tree.... */
     uint index = startVal;
     uint d = (startVal + 1) / 2;
     if (i > size * 2) {
       test[0]->pvalues[irun] = 0;
       return(0);
     }
     do {  /* While this path has not yet found a blank node to mark.... */
       ret = insertBit(x & 0x01, array, &index, &d);  /* Keep going */
       x >>= 1;
       if (--bitCount == 0) {
         x = gsl_rng_get(rng);
         bitCount = rmax_bits;
       }
     } while (ret == -2);  /* End of path. */

     i++;
   } while (ret == -1);  /* Couldn't insert marker; end of this tree. */
   positionCounts[ret/2]++;

   counts[i-1]++;
 }

 /* First p-value is calculated based on the targetData array. */
 test[0]->pvalues[irun] = chisq_pearson(counts + start, expected + start, end - start);

 /* Second p-value is calculated against a uniform distribution. */
 for (i = 0; i < size/2; i++) expected[i] = test[0]->tsamples/(size/2);
 test[1]->pvalues[irun] = chisq_uint_uniform_gtest(positionCounts, test[0]->tsamples, size/2);


 nullfree(positionCounts);
 nullfree(expected);
 nullfree(counts);
 nullfree(array);

 return(0);
}

/* 
 * Insert a bit into the tree, represented by an array.
 * A value of one is marked; zero is unmarked.
 * The function returns -2 is still on the path.
 * The function returns -1 if the path ends by marking a node.
 * The function returns >= 0 if the path went too deep; the
 * returned value is the last position of the path.
 */
static inline int insertBit(uint x, uchar *array, uint *i, uint *d) {
 if (x != 0) {
   *i += *d;
 } else {
   *i -= *d;
 }
 *d /= 2;

 if (array[*i] == 0) {
   array[*i] = 1;
   return -1;
 }
 if (*d == 0) return *i;
 else return -2;
}

