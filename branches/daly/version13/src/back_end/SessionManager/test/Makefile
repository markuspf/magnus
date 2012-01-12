IN=${SRC}/backend/SessionManager/test
MID=${INT}/backend/SessionManager/test
OUT=${OBJ}/${SYS}/backend/SessionManager/test
DOC=${MNT}/${SYS}/doc/test/backend/SessionManager/test
BIN=${MNT}/${SYS}/bin


FILES=${OUT}/magnus.o

DOCFILES=${DOC}/magnus.C.dvi

BINFILES=${BIN}/magnus

all: ${FILES} ${DOCFILES} ${BINFILES}
	@echo 5 finished ${IN}

${MID}/magnus.C: ${IN}/magnus.C.pamphlet
	@echo 1 making ${MID}/magnus.C from ${IN}/magnus.C.pamphlet
	@${DOCUMENT} src magnus.C ${MID}

${OUT}/magnus.o: ${MID}/magnus.C
	@echo 2 making ${OUT}/magnus.o from ${MID}magnus.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/magnus.o ${MID}/magnus.C

${DOC}/magnus.C.dvi: ${IN}/magnus.C.pamphlet ${DOC}/axiom.sty
	@echo 3 making ${DOC}/magnus.C.dvi from ${IN}/magnus.C.pamphlet 
	@${DOCUMENT} dvi magnus.C ${DOC} magnus.C ${NOISE}

${BIN}/magnus: ${OUT}/magnus.o
	@echo 4 making ${BIN}/magnus from ${OUT}/magnus.o
	@(cd ${OUT} ; \
	gcc ${CCFLAGS} ${INCLUDE} -o magnus ${OUT}/magnus.o ; \
	cp magnus ${BIN} )


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 4 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 

