##
##   $Id: Compile.mk 4 2005-01-07 00:28:20Z daly $
##

## Copyright (C) 1994 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: GNU Makefile for TietzeTrek
##
## Authors: Stephane Collart & Roger Needham
##
## Status: Useable.
##
## Revision History:
##


TietzeTrek: $(OBJ)
	${CC} ${OBJ} -o TietzeTrek

%.o: %.C
	${CC} ${CFLAGS} -c $< -o $@

# How to make the dependancy files:
# The pipe is put in its own shell to catch error conditions of the compiler
%.d: %.C
	$(SHELL) -ec '$(CC) -M $(INCLUDE) $< | sed "s:$*.o:& $@:g"' > $@

# Include all the dependancy files:
# Note that this causes Gmake to automatically remake them if they are
# out of date, and then reread them
include $(foreach file,$(FILES),$(file).d)
