#!/bin/sh

./autogen.sh
CFLAGS=-fcommon LDFLAGS=-fcommon ./configure --prefix=/usr
make -j8 || make
