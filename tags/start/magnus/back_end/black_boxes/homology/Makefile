##
##   $Id: Makefile 4 2005-01-07 00:28:20Z daly $
##

## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Makefile for the Magnus edition of
##           the Homology C Library
##
## Principal Author: Roger Needham
##
## Status: Useable.
##
## Revision History:
##


.PHONY: all clean distclean mostlyclean realclean

all: 
	if [ ! -d ./bin ]; then mkdir ./bin; fi
	cd lib; $(MAKE)
	cd src/homology; $(MAKE)
	strip bin/cb* bin/ch* bin/cmat bin/r* bin/tuples

clean: mostlyclean
	-rm -f bin/*

mostlyclean: 
	-rm -f lib/*.o lib/*.a
##	-rm -f src//homology/calchom

