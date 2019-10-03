# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([dieharder], [2.24.4])
AC_CONFIG_SRCDIR([dieharder.abs])
RELEASE=0
AC_SUBST(RELEASE)

AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_DEFINE([RELEASED], [], [Description])

dnl Library versioning (current:revision:age)
dnl See the libtool manual for an explanation of the numbers
dnl
dnl dieharder-1.0    libdieharder 0:0:0

DIEHARDER_LT_VERSION="0:0:0"
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

# Check for which system.
AC_CANONICAL_HOST

# Checks for programs.
AC_LANG(C)
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Disable unnecessary libtool tests for c++,fortran,java
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])
define([AC_LIBTOOL_LANG_GCJ_CONFIG], [:])
AC_PROG_LIBTOOL

# Check compiler features
AC_TYPE_SIZE_T
# AC_C_CONST
AC_C_VOLATILE
AC_C_INLINE

DIEHARDER_CFLAGS="-I$includedir"
DIEHARDER_LIBS="-L$libdir -ldieharder"

AC_SUBST(DIEHARDER_CFLAGS)
AC_SUBST(DIEHARDER_LIBS)

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldieharder':
AC_CHECK_LIB([dieharder], [main])
# FIXME: Replace `main' with a function in `-lgsl':
AC_CHECK_LIB([gsl], [main])
# FIXME: Replace `main' with a function in `-lgslcblas':
AC_CHECK_LIB([gslcblas], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

AC_CONFIG_FILES([
 dieharder_version.h 
 dieharder.spec
 dieharder/Makefile 
 libdieharder/Makefile 
 RDieHarder/DESCRIPTION
 Makefile])
AC_OUTPUT
