IN=${SRC}/backend/Todd-Coxeter/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Todd-Coxeter/include


FILES= ${OUT}/CosetEnumerator.h ${OUT}/CosetRelations.h \
       ${OUT}/CosetTables.h ${OUT}/EasyList.h ${OUT}/HavasTC.h

DOCFILES= ${DOC}/CosetEnumerator.h.dvi ${DOC}/CosetRelations.h.dvi \
          ${DOC}/CosetTables.h.dvi ${DOC}/EasyList.h.dvi ${DOC}/HavasTC.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 12 finished ${IN}

${OUT}/CosetEnumerator.h: ${IN}/CosetEnumerator.h.pamphlet
	@echo 1 making ${OUT}/CosetEnumerator.h from ${IN}/CosetEnumerator.h.pamphlet
	@${DOCUMENT} src CosetEnumerator.h ${OUT}

${DOC}/CosetEnumerator.h.dvi: ${IN}/CosetEnumerator.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/CosetEnumerator.h.dvi from ${IN}/CosetEnumerator.h.pamphlet 
	@${DOCUMENT} dvi CosetEnumerator.h ${DOC} CosetEnumerator.h ${NOISE}


${OUT}/CosetRelations.h: ${IN}/CosetRelations.h.pamphlet
	@echo 3 making ${OUT}/CosetRelations.h from ${IN}/CosetRelations.h.pamphlet
	@${DOCUMENT} src CosetRelations.h ${OUT}

${DOC}/CosetRelations.h.dvi: ${IN}/CosetRelations.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/CosetRelations.h.dvi from ${IN}/CosetRelations.h.pamphlet 
	@${DOCUMENT} dvi CosetRelations.h ${DOC} CosetRelations.h ${NOISE}


${OUT}/CosetTables.h: ${IN}/CosetTables.h.pamphlet
	@echo 5 making ${OUT}/CosetTables.h from ${IN}/CosetTables.h.pamphlet
	@${DOCUMENT} src CosetTables.h ${OUT}

${DOC}/CosetTables.h.dvi: ${IN}/CosetTables.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/CosetTables.h.dvi from ${IN}/CosetTables.h.pamphlet 
	@${DOCUMENT} dvi CosetTables.h ${DOC} CosetTables.h ${NOISE}


${OUT}/EasyList.h: ${IN}/EasyList.h.pamphlet
	@echo 7 making ${OUT}/EasyList.h from ${IN}/EasyList.h.pamphlet
	@${DOCUMENT} src EasyList.h ${OUT}

${DOC}/EasyList.h.dvi: ${IN}/EasyList.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/EasyList.h.dvi from ${IN}/EasyList.h.pamphlet 
	@${DOCUMENT} dvi EasyList.h ${DOC} EasyList.h ${NOISE}


${OUT}/HavasTC.h: ${IN}/HavasTC.h.pamphlet
	@echo 9 making ${OUT}/HavasTC.h from ${IN}/HavasTC.h.pamphlet
	@${DOCUMENT} src HavasTC.h ${OUT}

${DOC}/HavasTC.h.dvi: ${IN}/HavasTC.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/HavasTC.h.dvi from ${IN}/HavasTC.h.pamphlet 
	@${DOCUMENT} dvi HavasTC.h ${DOC} HavasTC.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 11 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
