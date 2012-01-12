
ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${MID} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE} 

DIRS=includedir 

all: ${DIRS}
	@echo 3 finished ${SRC}/backend/SessionManager

includedir:
	@echo 1 making ${SRC}/backend/SessionManager/include
	@mkdir -p ${SRC}/backend/SessionManager/include
	@mkdir -p ${INT}/include
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/SessionManager/include
	@(cd include ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

