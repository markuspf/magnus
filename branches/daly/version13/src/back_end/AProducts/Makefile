
ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${MID} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE} 

DIRS= includedir 
all: ${DIRS}

includedir:
	@echo 1 making ${SRC}/backend/AProducts/include
	@mkdir -p ${INT}/backend/AProducts/include
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/AProducts/include
	@(cd include ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

