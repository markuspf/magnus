IN=${SRC}/backend/SessionManager/src
MID=${INT}/backend/SessionManager/src
OUT=${OBJ}/${SYS}/backend/SessionManager/src
DOC=${MNT}/${SYS}/doc/src/backend/SessionManager/src
BIN=${MNT}/${SYS}/bin


FILES=${OUT}/SessionManager.o

DOCFILES=${DOC}/SessionManager.C.dvi

BINFILES=${BIN}/SessionManager

all: ${FILES} ${DOCFILES} ${BINFILES}
	@echo 5 finished ${IN}

${MID}/SessionManager.C: ${IN}/SessionManager.C.pamphlet
	@echo 1 making ${MID}/SessionManager.C from ${IN}/SessionManager.C.pamphlet
	@${DOCUMENT} src SessionManager.C ${MID}

${OUT}/SessionManager.o: ${MID}/SessionManager.C
	@echo 2 making ${OUT}/SessionManager.o from ${MID}SessionManager.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/SessionManager.o ${MID}/SessionManager.C

${DOC}/SessionManager.C.dvi: ${IN}/SessionManager.C.pamphlet ${DOC}/axiom.sty
	@echo 3 making ${DOC}/SessionManager.C.dvi from ${IN}/SessionManager.C.pamphlet 
	@${DOCUMENT} dvi SessionManager.C ${DOC} SessionManager.C ${NOISE}

${BIN}/SessionManager: ${OUT}/SessionManager.o
	@echo 4 making ${BIN}/SessionManager from ${OUT}/SessionManager.o
	@(cd ${OUT} ; \
	gcc ${CCFLAGS} ${INCLUDE} -o SessionManager ${OUT}/SessionManager.o ; \
	cp SessionManager ${BIN} )


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 4 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 

