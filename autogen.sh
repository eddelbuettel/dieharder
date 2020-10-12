#!/bin/sh

# Run this to generate all the auto-generated files needed by the GNU
# configure program.  This must be done FIRST; note well that there is
# no Makefile to run until it is done.

echo "Running autoreconf..."
autoreconf --install
