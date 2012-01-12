IN=${SRC}/backend/libg++
MID=${INT}/backend/libg++
OUT=${OBJ}/${SYS}/backend/libg++
DOC=${MNT}/${SYS}/doc/src/backend/libg++


ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${MID} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE} 


FILES=${OUT}/error.o     ${OUT}/fmtq.o ${OUT}/Intdouble.o \
      ${OUT}/Integer.o   ${OUT}/lg.o   ${OUT}/Rational.o  \
      ${OUT}/String.o

DOCFILES=${DOC}/error.C.dvi   ${DOC}/fmtq.C.dvi ${DOC}/Intdouble.C.dvi \
         ${DOC}/Integer.C.dvi ${DOC}/lg.C.dvi   ${DOC}/Rational.C.dvi  \
         ${DOC}/String.C.dvi

all: ${FILES} ${DOCFILES}
	@echo 23 finished ${SRC}/backend/libg++

${MID}/error.C: ${IN}/error.C.pamphlet
	@echo 1 making ${MID}/error.C from ${IN}/error.C.pamphlet
	@${DOCUMENT} src error.C ${MID}

${OUT}/error.o: ${MID}/error.C 
	@echo 2 making ${OUT}/error.o from ${MID}/error.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/error.o ${MID}/error.C 

${DOC}/error.C.dvi: ${IN}/error.C.pamphlet ${DOC}/axiom.sty
	@echo 3 making ${DOC}/error.C.dvi from ${IN}/error.C.pamphlet
	@${DOCUMENT} dvi error.C ${DOC} error.C ${NOISE}


${MID}/fmtq.C: ${IN}/fmtq.C.pamphlet
	@echo 4 making ${MID}/fmtq.C from ${IN}/fmtq.C.pamphlet
	@${DOCUMENT} src fmtq.C ${MID} 

${OUT}/fmtq.o: ${MID}/fmtq.C 
	@echo 5 making ${OUT}/fmtq.o from ${MID}/fmtq.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/fmtq.o ${MID}/fmtq.C 

${DOC}/fmtq.C.dvi: ${IN}/fmtq.C.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/fmtq.C.dvi from ${IN}/fmtq.C.pamphlet
	@${DOCUMENT} dvi fmtq.C ${DOC} fmtq.C ${NOISE}


${MID}/Intdouble.C: ${IN}/Intdouble.C.pamphlet
	@echo 7 making ${MID}/Intdouble.C from ${IN}/Intdouble.C.pamphlet
	@${DOCUMENT} src Intdouble.C ${MID}

${OUT}/Intdouble.o: ${MID}/Intdouble.C 
	@echo 8 making ${OUT}/Intdouble.o from ${MID}/Intdouble.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/Intdouble.o ${MID}/Intdouble.C 

${DOC}/Intdouble.C.dvi: ${IN}/Intdouble.C.pamphlet ${DOC}/axiom.sty
	@echo 9 making ${DOC}/Intdouble.C.dvi from ${IN}/Intdouble.C.pamphlet
	@${DOCUMENT} dvi Intdouble.C ${DOC} Intdouble.C ${NOISE}


${MID}/Integer.C: ${IN}/Integer.C.pamphlet
	@echo 10 making ${MID}/Integer.C from ${IN}/Integer.C.pamphlet
	@${DOCUMENT} src Integer.C ${MID}

${OUT}/Integer.o: ${MID}/Integer.C 
	@echo 11 making ${OUT}/Integer.o from ${MID}/Integer.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/Integer.o ${MID}/Integer.C 

${DOC}/Integer.C.dvi: ${IN}/Integer.C.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/Integer.C.dvi from ${IN}/Integer.C.pamphlet
	@${DOCUMENT} dvi Integer.C ${DOC} Integer.C ${NOISE}


${MID}/lg.C: ${IN}/lg.C.pamphlet
	@echo 13 making ${MID}/lg.C from ${IN}/lg.C.pamphlet
	@${DOCUMENT} src lg.C ${MID}

${OUT}/lg.o: ${MID}/lg.C 
	@echo 14 making ${OUT}/lg.o from ${MID}/lg.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/lg.o ${MID}/lg.C 

${DOC}/lg.C.dvi: ${IN}/lg.C.pamphlet ${DOC}/axiom.sty
	@echo 15 making ${DOC}/lg.C.dvi from ${IN}/lg.C.pamphlet
	@${DOCUMENT} dvi lg.C ${DOC} lg.C ${NOISE}


${MID}/Rational.C: ${IN}/Rational.C.pamphlet
	@echo 16 making ${MID}/Rational.C from ${IN}/Rational.C.pamphlet
	@${DOCUMENT} src Rational.C ${MID}

${OUT}/Rational.o: ${MID}/Rational.C 
	@echo 17 making ${OUT}/Rational.o from ${MID}/Rational.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/Rational.o ${MID}/Rational.C 

${DOC}/Rational.C.dvi: ${IN}/Rational.C.pamphlet ${DOC}/axiom.sty
	@echo 18 making ${DOC}/Rational.C.dvi from ${IN}/Rational.C.pamphlet
	@${DOCUMENT} dvi Rational.C ${DOC} Rational.C ${NOISE}


${MID}/String.C: ${IN}/String.C.pamphlet
	@echo 19 making ${MID}/String.C from ${IN}/String.C.pamphlet
	@${DOCUMENT} src String.C ${MID}

${OUT}/String.o: ${MID}/String.C 
	@echo 20 making ${OUT}/String.o from ${MID}/String.C 
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/String.o ${MID}/String.C 

${DOC}/String.C.dvi: ${IN}/String.C.pamphlet ${DOC}/axiom.sty
	@echo 21 making ${DOC}/String.C.dvi from ${IN}/String.C.pamphlet
	@${DOCUMENT} dvi String.C ${DOC} String.C ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 22 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 

