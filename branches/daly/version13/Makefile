MAG=$(shell pwd)
#SYS=$(notdir $(MAGNUS))
SYS=linux
SRC=${MAG}/src
INT=${MAG}/int
OBJ=${MAG}/obj
MNT=${MAG}/mnt
BIN=${MNT}/${SYS}/bin
ZIPS=${MAG}/zips
TMP=${OBJ}/tmp
NOISE=${TMP}/noise
INC=${INT}/include
CCFLAGS=-DHAVE_UNISTD_H -DDEBUG -g -fno-operator-names -D_G_NO_NAV \
  -Wno-deprecated -DTPD -D_CPP_BACKWARD_BACKWARD_WARNING_H -DSAFETY=2 
#INCLUDE=-I${INC} -I${INT} -I/usr/include/g++-3
INCLUDE=-I${INC} -I${INT}
DOCUMENT=${BIN}/document
TANGLE=${BIN}/notangle
WEAVE=${BIN}/noweave

ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${INC} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE}

all: noweb litcmds
	@echo 15 making ${SRC}
	@echo 16 making ${SRC}/Makefile.${SYS}
	@${DOCUMENT} chunk Makefile.${SYS} Makefile . Makefile.${SYS}
	@${DOCUMENT} dvi Makefile . Makefile ${NOISE}
	@ ${ENV} ${MAKE} -f Makefile.${SYS}
	@echo 17 finished system build on `date` | tee >lastBuildDate

start: noweb litcmds

noweb:
	@echo 2 making noweb
	@mkdir -p ${TMP}
	@mkdir -p ${OBJ}/noweb
	@mkdir -p ${BIN}/lib
	@mkdir -p ${BIN}/man
	@mkdir -p ${BIN}/tex/inputs
	@( cd ${OBJ}/noweb ; \
	tar -zxf ${ZIPS}/noweb-2.10a.tgz ; \
	cd ${OBJ}/noweb/src ; \
	./awkname gawk ; \
	cd ${OBJ}/noweb/src/c ; \
	echo 3 applying patch zips/noweb.modules.c.patch ; \
	patch <${ZIPS}/noweb.modules.c.patch ; \
	cd ${OBJ}/noweb/src ; \
	echo 4 applying patch zips/noweb.src.Makefile.patch ; \
	patch <${ZIPS}/noweb.src.Makefile.patch ; \
	cd ${OBJ}/noweb/src ; \
	echo 5 applying patch zips/noweb.src.Makefile.2.patch ; \
	patch <${ZIPS}/noweb.src.Makefile.2.patch ; \
	cd ${OBJ}/noweb/src ; \
	echo 6 making noweb, output redirected to ${TMP}/noweb.output ; \
	${ENV} ${MAKE} all install >${TMP}/noweb.output )
	@echo The file marks the fact that noweb has been made > noweb

nowebclean:
	@echo 8 cleaning ${OBJ}/noweb
	@rm -rf ${OBJ}/noweb
	@rm -f noweb

litcmds:
	@echo 9 Environment ${ENV}
	@echo 10 copying ${SRC}/scripts to ${BIN}
	@cp -pr ${SRC}/scripts/* ${BIN}


document: noweb litcmds
	@echo 18 making a ${SYS} system
	@echo 19 Environment ${ENV}
	@ ${DOCUMENT} chunk Makefile.${SYS} Makefile . Makefile.${SYS}
	@ ${ENV} ${MAKE} -f Makefile.${SYS} document

clean: noweb litcmds
	@echo 20 cleaning up
	@ ${DOCUMENT} chunk Makefile.${SYS} Makefile . Makefile.${SYS}
	@ ${ENV} ${MAKE} -f Makefile.${SYS} clean
	@ rm -f noweb
	@ rm -f *~

