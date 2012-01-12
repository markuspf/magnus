##
##   $Id: Makefile 4 2005-01-07 00:28:20Z daly $
##

## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Makefile for the Magnus edition of
##           the Rutgers Knuth-Bendix Package
##
## Principal Author: Roger Needham
##
## Status: Useable.
##
## Revision History:
##


.PHONY: all clean distclean mostlyclean realclean

all : bin/rkbp
VPATH = ./source
c_files := $(wildcard ./source/*.C)
h_files := $(wildcard ./source/*.h)

bin/rkbp : $(h_files) $(c_files)
	if [ ! -d ./bin ]; then mkdir ./bin; fi
	cd source; g++ -O -o ../bin/rkbp *.C
	strip bin/rkbp
clean:
	-rm -f bin/rkbp


mostlyclean:

