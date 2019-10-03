#========================================================================
# This is a Makefile for the libdieharder library, part of the
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
DIR = libdieharder

SVNTREE = $(HOME)/Src/svn-tree
SVNPATH = $(SVNTREE)/$(PROJECT)
SVNTIME = $(DIR:=.svn.time)

# This is automagically set in the toplevel build.  Do not edit by
# hand.
VERSION=@VERSION@
VERSION_MAJOR = $(shell expr $(VERSION) : '\(.*\)\..*\..*' \| $(VERSION))
RELEASE=0

# This is the library from which both wulfstat and wulflogger (and maybe
# other stuff in the future) is built.
PROGMAN = libdieharder.3
# (FC "requires" no static library build)
# PROGLIB_A = libdieharder.a
PROGLIB_SO = libdieharder.so
PROGLIB_SONAME = libdieharder.so.$(VERSION_MAJOR)
PROGLIB_SO_VERSION = libdieharder.so.$(VERSION)

#========================================================================
# Define all sources.  Note that we choose to depend on ALL the includes
# in the include directory, which cannot be empty or this will barf.
#========================================================================
LIBINCLUDES = $(shell ls ../include/dieharder/*.h  2>&1 | sed -e "/\/bin\/ls:/d")
LIBSOURCES = $(shell ls *.c  2>&1 | sed -e "/\/bin\/ls:/d")
LIBOBJECTS = $(LIBSOURCES:.c=.o)

DEFINES = -DVERSION=$(VERSION)

#========================================================================
# Define parameters and directives needed in compile/link steps.
#========================================================================
# C Compiler
CC = gcc

# Compile flags (use fairly standard -O3 as default)
CFLAGS = -O3 -I ../include $(DEFINES) 

# Linker flags
LDFLAGS = -g -fpic -shared -Wl,-soname,$(PROGLIB_SONAME)

# Libraries
LIBS = -lgsl -lgslcblas -lm

#========================================================================
# List of variants one can make.  all is the default.  We always
# presume the simplest of dependencies and remake if includes change
# for example.
#========================================================================
all: $(PROGLIB_SO_VERSION) # $(PROGLIB_A)

# FC "requires" no static lib build/install	
# $(PROGLIB_A): $(INCTIME) $(LIBOBJECTS) $(LIBINCLUDES)
# 	ar r $(PROGLIB_A) $(LIBOBJECTS)
# 	ranlib $(PROGLIB_A)

$(PROGLIB_SO_VERSION): $(LIBOBJECTS) $(LIBINCLUDES)
	gcc $(LDFLAGS) -o $(PROGLIB_SO_VERSION) $(LIBOBJECTS)
	ldconfig -n .
	ln -sf $(PROGLIB_SO_VERSION) $(PROGLIB_SO)

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
	$(LPR) $(LIBSOURCES) $(LIBINCLUDES)

#========================================================================
#  A standard cleanup target
#========================================================================
clean:
	rm -f core $(PROGLIB_SO)* $(LIBOBJECTS) $(PROGMAN).gz

#========================================================================
# This is critical.  For the toplevel rpm build to succeed,
#  make prefix=/usr install
# (run by rpmbuild from the specfile) has to work, in the right order.
# This target has to install precisely the files required by the
# specfile for the dieharder package, in precisely the right locations.
#========================================================================
install: $(PROGLIB_SO_VERSION) $(PROGLIB_A) $(PROGMAN)
	(install -d $(libdir); \
	install -m 755 $(PROGLIB_SO_VERSION) $(libdir)/$(PROGLIB_SO_VERSION); \
        ln -sf $(libdir)/$(PROGLIB_SO_VERSION) $(libdir)/$(PROGLIB_SO); \
	install -d $(includedir)/dieharder; \
	install -m 644 ../include/dieharder/*.h $(includedir)/dieharder; \
	install -d $(prefix)/share/man/man3; \
	gzip -c -9 $(PROGMAN) > $(PROGMAN).gz; \
	install -m 644 $(DIR).3.gz $(prefix)/share/man/man3)

# FC requires no static lib install
# 	install -m 755 $(PROGLIB_A) $(libdir)/$(PROGLIB_A); \
# ldconfig -n $(libdir); \
#========================================================================
# We give all generic rules below.  Currently we only need a rule for 
# objects.
#========================================================================
%.o:%.c $(LIBINCLUDES) Makefile
	$(CC) -fpic -c $(CFLAGS) $<
