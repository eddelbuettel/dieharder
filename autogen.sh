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
echo "Now use ./configure --prefix=/usr for a regular install"
