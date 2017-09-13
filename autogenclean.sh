#!/bin/bash -e
#
# deletes compile files, so that I can upload the whole folder to github
# use make clean first

find . -name "Makefile"  -exec rm {} \;
find . -name "Makefile.in"  -exec rm {} \;
find . -name "autom4te.cache"  -exec rm -r {} \;
find . -name ".deps"  -exec rm -r {} \;
find . -name "m4"  -exec rm -r {} \;
find . -name "*.gmo"  -exec rm {} \;
find . -name "POTFILES"  -exec rm {} \;
find . -name "Makefile.in.in"  -exec rm {} \;
find . -name "stamp-it"  -exec rm {} \;
rm ./sensors-applet/config.h*;
rm ./sensors-applet/stamp-h1;
rm ./aclocal.m4 ./compile ./config.* ./configure ./depcomp ./install-sh ./libtool ./ltmain.sh ./missing ./omf.make ./xmldocs.make ./INSTALL;
