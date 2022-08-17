#include <dieharder/libdieharder.h>

#include <stdint.h>    /* uint32_t, uintmax_t */
#include <inttypes.h>  /* SCNuMAX */
#include <stdio.h>     /* printf */

#define PREDICT_MERSENNE_SAMPLES_NEEDED ((uint32_t)624u)

static uint32_t const
	param_w = 32,              /* word size */
	param_m = 397,             /* middle term */
	param_r = 31,              /* separation point of one word */
	param_a = 0x9908b0df,      /* bottom row of matrix A */
	param_u = 11,              /* tempering shift */
	param_s = 7,               /* tempering shift */
	param_t = 15,              /* tempering shift */
	param_l = 18,              /* tempering shift */
	param_b = 0x9d2c5680,      /* tempering mask */
	param_c = 0xefc60000;      /* tempering mask */

static uint32_t Undo_Xor_Rshift(uint32_t const x, unsigned const shift)
{
	unsigned shift_amount;

	uint32_t result = x;

	for (shift_amount = shift; shift_amount < param_w; shift_amount += shift)
	{
		result ^= (x >> shift_amount);
	}

	return result;
}

static uint32_t Undo_Xor_Lshiftmask(uint32_t x, unsigned const shift, uint32_t const mask)
{
	unsigned i;

	uint32_t window = ((uint32_t)1u << shift) - 1u;

	for (i = 0; i < param_w / shift; ++i)
	{
		x ^= ((window & x) << shift) & mask;
		window <<= shift;
	}

	return x;
}

static uint32_t temper(uint32_t x)
{
	x ^= (x >> param_u);
	x ^= ((x << param_s) & param_b);
	x ^= ((x << param_t) & param_c);
	x ^= (x >> param_l);

	return x;
}

static uint32_t untemper(uint32_t x)
{
	x = Undo_Xor_Rshift(x, param_l);
	x = Undo_Xor_Lshiftmask(x, param_t, param_c);
	x = Undo_Xor_Lshiftmask(x, param_s, param_b);
	x = Undo_Xor_Rshift(x, param_u);

	return x;
}

static uint32_t upper(uint32_t const x)
{
	return x & 2147483648u;
}

static uint32_t lower(uint32_t const x)
{
	return x & 2147483647u;
}

static uint32_t timesA(uint32_t x)
{
	int const is_odd = x & 1u;

	x >>= 1u;

	if ( is_odd )
	{
		x ^= param_a;
	}

	return x;
}

static unsigned CircularAdvance(unsigned const offset, unsigned const advance)
{
	return (offset + advance) % PREDICT_MERSENNE_SAMPLES_NEEDED;
}

//#define verbose_printf(...) printf(__VA_ARGS__)
#define verbose_printf(...) /* nothing */
//define printf(...) /* nothing */

int diehard_predict_mersenne(Test **test, int irun)
{
	test[0]->ntuple = 0;           // 0 means "ignored"
	test[0]->pvalues[irun] = 0.0;  // 0.0 is a P-value for failure

	// Don't bother running the test if we don't have at least 8 test numbers
	if ( test[0]->tsamples < (2*PREDICT_MERSENNE_SAMPLES_NEEDED) ) return 0;

	uint32_t *const samples_seen_so_far = malloc(PREDICT_MERSENNE_SAMPLES_NEEDED * sizeof(*samples_seen_so_far));

	if ( !samples_seen_so_far ) return 0;  // if dynamic memory allocation error

	uintmax_t num_correct = 0, num_incorrect = 0;

	verbose_printf("Waiting for %" SCNuMAX " previous inputs\n", (uintmax_t)PREDICT_MERSENNE_SAMPLES_NEEDED);

	for( uintmax_t i = 0; i < PREDICT_MERSENNE_SAMPLES_NEEDED; ++i )
	{
		uint32_t tmp;
		get_rand_bits(&tmp,sizeof(uint32_t),32u,rng);
		samples_seen_so_far[i] = untemper(tmp);
	}

	verbose_printf("Ready to predict\n");

	verbose_printf("Third-last element == %" SCNuMAX "\n", (uintmax_t)samples_seen_so_far[PREDICT_MERSENNE_SAMPLES_NEEDED - 3u]);
	verbose_printf("Second-last element == %" SCNuMAX "\n", (uintmax_t)samples_seen_so_far[PREDICT_MERSENNE_SAMPLES_NEEDED - 2u]);
	verbose_printf("Last element == %" SCNuMAX "\n", (uintmax_t)samples_seen_so_far[PREDICT_MERSENNE_SAMPLES_NEEDED - 1u]);

	for ( uintmax_t count_samples = 0; count_samples < (test[0]->tsamples - PREDICT_MERSENNE_SAMPLES_NEEDED); ++count_samples )
	{
		uintmax_t const offset = count_samples % PREDICT_MERSENNE_SAMPLES_NEEDED;

		char const *status = 0;

		uint32_t predicted, actual;

		uint32_t const alpha = samples_seen_so_far[ CircularAdvance(offset,param_m) ];

		verbose_printf("Alpha == %" SCNuMAX "\n", (uintmax_t)alpha);
		verbose_printf("Seen[0] == %" SCNuMAX "\n", (uintmax_t)samples_seen_so_far[0]);
		verbose_printf("Seen[1] == %" SCNuMAX "\n", (uintmax_t)samples_seen_so_far[1]);
		verbose_printf("upper(Seen[0]) == %" SCNuMAX "\n", (uintmax_t)upper(samples_seen_so_far[0]));
		verbose_printf("lower(Seen[1]) == %" SCNuMAX "\n", (uintmax_t)lower(samples_seen_so_far[1]));

		uint32_t const prebeta =   upper(samples_seen_so_far[offset])
		                         | lower(samples_seen_so_far[CircularAdvance(offset,1u)]);

		verbose_printf("Prebeta == %" SCNuMAX "\n", (uintmax_t)prebeta);

		uint32_t const beta = timesA(prebeta);

		verbose_printf("Beta == %" SCNuMAX "\n", (uintmax_t)beta);

		uint32_t const next_val = alpha ^ beta;

		samples_seen_so_far[offset] = next_val;

		verbose_printf("Next_val == %" SCNuMAX "\n", (uintmax_t)next_val);

		predicted = temper(next_val);

		get_rand_bits(&actual,sizeof(uint32_t),32u,rng);

		if ( predicted == actual )
		{
			status = "CORRECT";
			++num_correct;
		}
		else
		{
			status = "INACCURATE";
			++num_incorrect;
		}

		verbose_printf("Sample Number: %" SCNuMAX " - Predicted %" SCNuMAX " got %" SCNuMAX " -- %s\n", count_samples, (uintmax_t)predicted, (uintmax_t)actual, status);
	}

	printf("Correct = %" SCNuMAX ", Incorrect = %" SCNuMAX "\n", num_correct, num_incorrect);

	if ( ((long double)num_correct / num_incorrect) > 0.007L )
	{
		test[0]->pvalues[irun] = 0.0;
	}
	else
	{
		test[0]->pvalues[irun] = 0.2;
	}

	free(samples_seen_so_far);

	return 0;
}
