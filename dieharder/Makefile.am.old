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
PROJECT = dieharder
PROGRAM = dieharder
DIR = $(PROGRAM)

SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/$(PROJECT)
SVNTIME = $(DIR:=.svn.time)

# This is automagically set in the toplevel build.  Do not edit by
# hand.
VERSION=@VERSION@

#========================================================================
# Define all sources.  Note that we choose to depend on ALL the includes
# in the include directory, which cannot be empty or this will barf.
# We also have dependencies that extend to the libdieharder source
# directory, as we need to force a build if any of them change.
#========================================================================
SRCSOURCES = $(shell ls *.c  2>&1 | sed -e "/\/bin\/ls:/d")
SRCINCLUDES = $(shell ls *.h  2>&1 | sed -e "/\/bin\/ls:/d")

# These are important dependencies, although it isn't easy to
# see how to force a consistent build when working on the UI
# and library at the same time.  Maybe with particular make targets...
LIBINCLUDES = $(shell ls ../include/dieharder/*.h  2>&1 | sed -e "/\/bin\/ls:/d")
PROGLIB_SONAME = ../libwulf/libdieharder.so.$(VERSION)

SOURCES = $(LIBSOURCES) $(SRCSOURCES)
INCLUDES = $(LIBINCLUDES) $(SRCINCLUDES)
OBJECTS = $(SRCSOURCES:.c=.o)

DEFINES = -DVERSION=$(VERSION)

#========================================================================
# Define parameters and directives needed in compile/link steps.
#========================================================================
# C Compiler
# CC = gcc

# Compile flags (use fairly standard -O3 as default)
AM_CPPFLAGS = -I ${top_srcdir}/include $(DEFINES) -I ${includedir}
AM_CFLAGS = -O3

# Libraries
LIBS += -L ${top_srcdir}/libdieharder -ldieharder -lgsl -lgslcblas -lm

#========================================================================
# List of variants one can make.  all is the default.  We always
# presume the simplest of dependencies and remake if includes change
# for example.
#========================================================================
all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) $(PROGLIB_SONAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS) 

#========================================================================
# This is cautiously permitted.  REALLY, though, we need a conditional
# that punts with a warning if we try this during an rpm build and
# the library is not found -- the right solution then is to install
# the library first!
#========================================================================
$(PROGLIB_SONAME):
	- (cd ../libdieharder ;\
	 $(MAKE))

#========================================================================
# The only safe place to do commits is in the toplevel directory
#========================================================================
svn:
	(cd ..;make svn)

sync:
	(cd ..;make sync)

#========================================================================
# printout makes an enscript -2r printout of SOURCES and
# and INCLUDES.  Use lpr if you don't have enscript
#========================================================================
LPR = enscript -2r
# LPR = lpr
printout:
	$(LPR) $(SRCSOURCES) $(SRCINCLUDES)


install : $(PROGRAM)
	(install -d $(DESTDIR)$(bindir); \
	install -m 755 $(PROGRAM) $(DESTDIR)$(bindir); \
	install -d $(DESTDIR)$(mandir)/man1; \
	install -m 644 $(PROGRAM).1 $(DESTDIR)$(mandir)/man1)

#========================================================================
# We give all generic rules below.  Currently we only need a rule for 
# objects.
#========================================================================
.o:.c
	$(CC) -c $(CFLAGS) $<

