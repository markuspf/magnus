
ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${MID} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE} 

DIRS= includedir 
all: ${DIRS}
	echo 2 finished ${SRC}/backend/general

includedir:
	@echo 1 making ${SRC}/backend/general/include
	@mkdir -p ${INT}/backend/general/include
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/general/include
	@(cd include ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

