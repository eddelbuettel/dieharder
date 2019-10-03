#==================================================================
# Process this file with autoconf to produce a configure script.
# Make changes to the version number(s) in particular ONLY here.
#==================================================================

AC_PREREQ(2.59)

#==================================================================
# Be sure to set DIEHARDER_LT_VERSION below at the same time
# you change this:
#==================================================================
AC_INIT([dieharder], [2.27.12])

AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([dieharder.abs])
RELEASE=1
AC_SUBST(RELEASE)

AC_CONFIG_HEADERS([config.h])
# AM_MAINTAINER_MODE

AC_DEFINE([RELEASED], [], [Description])

#==================================================================
# Library versioning (current:revision:age)
# See the libtool manual for an explanation of the numbers.
# NOTE WELL!  One MUST bump LT_VERSION along with AC_INIT,
# apparently.
# 
# dieharder-1.0    libdieharder 0:0:0
#==================================================================
LT_INIT
AC_SUBST([LIBTOOL_DEPS])
DIEHARDER_LT_VERSION="2:27:12"
AC_SUBST(DIEHARDER_LT_VERSION)

case "$VERSION" in
    *+)
        AC_DEFINE(RELEASED,0)
        ;;
    *)
        AC_DEFINE(RELEASED,1)
        ;;
esac
AC_SUBST(RELEASED)      

#==================================================================
# Check for which system.
#==================================================================
AC_CANONICAL_HOST

#==================================================================
# Checks for programs.
#==================================================================
AC_LANG(C)
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

#==================================================================
# OK, these five macros prevent my personal path from appearing
# in the default Makefile, and actually should make it work to
# rebuilt itself even if somebody completely blows off running
# ./autogen.sh.  And it ALMOST WORKS
#==================================================================
ACLOCAL="${SHELL} ./missing --run aclocal"
ACTAR="${SHELL} ./missing --run tar"
AUTOCONF="${SHELL} ./missing --run autoconf;echo \"Run ./configure\";exit"
AUTOHEADER="${SHELL} ./missing --run autoheader"
AUTOMAKE="${SHELL} ./missing --run automake --add-missing --copy --gnu"

AC_SUBST(ACLOCAL)
AC_SUBST(ACTAR)
AC_SUBST(AUTOCONF)
AC_SUBST(AUTOHEADER)
AC_SUBST(AUTOMAKE)

#==================================================================
# Disable unnecessary libtool tests for c++,fortran,java
#==================================================================
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])
define([AC_LIBTOOL_LANG_GCJ_CONFIG], [:])
AC_PROG_LIBTOOL

#==================================================================
# Check compiler features
#==================================================================
AC_TYPE_SIZE_T
# AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

DIEHARDER_CFLAGS="-I$includedir"
DIEHARDER_LIBS="-L$libdir -ldieharder"

AC_SUBST(DIEHARDER_CFLAGS)
AC_SUBST(DIEHARDER_LIBS)

#==================================================================
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lgsl':
#==================================================================
AC_CHECK_LIB([gsl], [main])
# FIXME: Replace `main' with a function in `-lgslcblas':
AC_CHECK_LIB([gslcblas], [main])

#==================================================================
# Checks for typedefs, structures, and compiler characteristics.
#==================================================================
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

#==================================================================
# Files that configure needs to process with this so that they are
# all happy and macro-replaced.
#==================================================================
AC_CONFIG_FILES([
 dieharder_version.h 
 dieharder.spec
 dieharder/Makefile 
 include/Makefile
 libdieharder/Makefile 
 Makefile])

AC_OUTPUT
