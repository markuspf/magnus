## Copyright (C) 1996 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Makefile for the magnus back end.
##
## Principal Author: Roger Needham
##
## Status: Useable.
##
## Revision History:
##

## The following may be defined by a higher-level make:

ifeq (0,$(MAKELEVEL))
  CC = g++
  DEBUG = -DDEBUG -g
  DEFINE = -DHAVE_UNISTD_H
  CFLAGS = $(DEBUG) $(DEFINE)
endif

CFLAGS += $(INCLUDE)

export CC
export DEBUG
export DEFINE
export CFLAGS


SRC_DIRS = general


.PHONY: all lib clean 

all: 
	@if [ ! -f global/config.h ]; \
         then echo; echo "You must run ./Configure first"; echo; exit 1; fi

	cd libg++; $(MAKE) all
	cd general; $(MAKE) all
	cd Statistics; $(MAKE) all
	cd SMApps; $(MAKE) catlib
	cd SessionManager; $(MAKE) catlib
	cd SessionManager; $(MAKE) magnus

lib:
	cd libg++; $(MAKE) lib
	cd general; $(MAKE) lib
	cd Statistics; $(MAKE) lib
	cd SMApps; $(MAKE) catlib
	cd SessionManager; $(MAKE) catlib

clean:
	cd libg++; $(MAKE) clean
	cd general; $(MAKE) clean
	cd Statistics; $(MAKE) clean
	cd SMApps; $(MAKE) clean
	cd SessionManager; $(MAKE) clean
	-rm lib/*
