find -name Makefile\.in | xargs -i -r -n1 git restore "{}"
git restore ./manual/Makefile ./aclocal.m4 ./configure
