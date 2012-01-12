IN=${SRC}/backend/Statistics/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Statistics/include


FILES= ${OUT}/DataSet.h

DOCFILES= ${DOC}/DataSet.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 78 finished ${IN}

${OUT}/DataSet.h: ${IN}/DataSet.h.pamphlet
	@echo 73 making ${OUT}/DataSet.h from ${IN}/DataSet.h.pamphlet
	@${DOCUMENT} src DataSet.h ${OUT}

${DOC}/DataSet.h.dvi: ${IN}/DataSet.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/DataSet.h.dvi from ${IN}/DataSet.h.pamphlet 
	@${DOCUMENT} dvi DataSet.h ${DOC} DataSet.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 77 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
