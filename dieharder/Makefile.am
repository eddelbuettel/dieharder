#========================================================================
# This is a Makefile for the dieharder tty application, part of the
# overall dieharder package.  It has a very abbreviated set of targets.
#
# make          alone should build the application.
#
# make clean    deletes the application and all object files
# make install  installs application and a man page
# make printout prints out all source and include files
#
# These two commands execute in the toplevel project directory only.
# make svn      does a svn commit and creates the timestamp $(SVN)
# make sync     does a svn commit and rsyncs to list of hosts
#========================================================================

# This is automagically set in the toplevel build.  Do not edit by
# hand.
VERSION=@VERSION@

#========================================================================
# Define all sources.  Note that this little trick will get ALL
# the source files in the working directory, which must not be empty
# Alas, GBT complains just a bit about the shell commands...
#========================================================================
# dieharder_SOURCES = $(shell ls *.c  2>&1 | sed -e "/\/bin\/ls:/d")
# SRCINCLUDES = $(shell ls *.h  2>&1 | sed -e "/\/bin\/ls:/d")
bin_PROGRAMS = dieharder
man1_MANS = dieharder.1
dieharder_LDADD = ../libdieharder/libdieharder.la -lgsl -lgslcblas -lm
dieharder_SOURCES = \
	dieharder.c \
	dieharder_exit.c \
	help.c \
	histogram.c \
	list_rand.c \
	list_rngs.c \
	list_tests.c \
	output.c output.h \
	output_rnds.c \
	parsecl.c \
	rdieharder.c \
	rng_empty_random.c \
	run_diehard_2dsphere.c \
	run_diehard_3dsphere.c \
	run_diehard_birthdays.c \
	run_diehard_bitstream.c \
	run_diehard_count_1s_byte.c \
	run_diehard_count_1s_stream.c \
	run_diehard_craps.c \
	run_diehard_dna.c \
	run_diehard_operm5.c \
	run_diehard_opso.c \
	run_diehard_oqso.c \
	run_diehard_parking_lot.c \
	run_diehard_rank_32x32.c \
	run_diehard_rank_6x8.c \
	run_diehard_runs.c \
	run_diehard_squeeze.c \
	run_diehard_sums.c \
	run_marsaglia_tsang_gcd.c \
	run_rgb_bitdist.c \
	run_rgb_lagged_sums.c \
	run_rgb_minimum_distance.c \
	run_rgb_operm.c \
	run_rgb_permutations.c \
	run_rgb_persist.c \
	run_rgb_timing.c \
	run_sts_monobit.c \
	run_sts_runs.c \
	run_sts_serial.c \
	run_user_template.c \
	startup.c \
	testbits.c \
	user_template.c \
	work.c

DEFINES = -DVERSION=$(VERSION)

#========================================================================
# Define parameters and directives needed in compile/link steps.
#========================================================================
# C Compiler
# CC = gcc

# Compile flags (use fairly standard -O3 as default)
AM_CPPFLAGS = -I ${top_srcdir}/include $(DEFINES) -I ${includedir}
AM_CFLAGS = -O3

#========================================================================
# The only safe place to do svn commits is in the toplevel directory
#========================================================================
SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/dieharder
SVNTIME = dieharder.svn.time)
svn:
	(cd ..;make svn)

sync:
	(cd ..;make sync)
