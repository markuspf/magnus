##
##   $Id: Makefile 4 2005-01-07 00:28:20Z daly $
##

## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Makefile for the Magnus edition of Dion Gildenhuys'
##           one relator word problem program
##
## Principal Author: Roger Needham
##
## Status: Useable.
##
## Revision History:
##


.PHONY: all clean distclean mostlyclean realclean

all: orwp

orwp : orwp.C
	g++ -O2 -o orwp orwp.C
	strip orwp

clean:
	-rm -f orwp

mostlyclean: 