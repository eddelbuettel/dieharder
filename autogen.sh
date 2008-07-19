#! /bin/sh

# Run this to generate all the auto-generated files needed by the GNU
# configure program

# libtoolize --automake

autoheader
aclocal
rm -f Makefile Makefile.in dieharder/Makefile dieharder/Makefile.in \
   libdieharder/Makefile libdieharder/Makefile.in
automake --add-missing --gnu
autoconf

# Might as well run this
./configure --prefix=/usr
echo "./configure --prefix=/usr completed.  Rerun with different"
echo "--prefix if you want to put it someplace else.  Otherwise"
echo "you should be able to make, make install, or make rpm."
