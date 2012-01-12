IN=${SRC}/backend/Map/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Map/include


FILES= ${OUT}/Automorphism.h ${OUT}/Endomorphism.h \
       ${OUT}/MapEnum.h ${OUT}/Map.h ${OUT}/MapParser.h \
       ${OUT}/RandomAutoInFree.h

DOCFILES= ${DOC}/Automorphism.h.dvi ${DOC}/Endomorphism.h.dvi \
          ${DOC}/MapEnum.h.dvi ${DOC}/Map.h.dvi ${DOC}/MapParser.h.dvi \
          ${DOC}/RandomAutoInFree.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 14 finished ${IN}

${OUT}/Automorphism.h: ${IN}/Automorphism.h.pamphlet
	@echo 1 making ${OUT}/Automorphism.h from ${IN}/Automorphism.h.pamphlet
	@${DOCUMENT} src Automorphism.h ${OUT}

${DOC}/Automorphism.h.dvi: ${IN}/Automorphism.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/Automorphism.h.dvi from ${IN}/Automorphism.h.pamphlet 
	@${DOCUMENT} dvi Automorphism.h ${DOC} Automorphism.h ${NOISE}


${OUT}/Endomorphism.h: ${IN}/Endomorphism.h.pamphlet
	@echo 3 making ${OUT}/Endomorphism.h from ${IN}/Endomorphism.h.pamphlet
	@${DOCUMENT} src Endomorphism.h ${OUT}

${DOC}/Endomorphism.h.dvi: ${IN}/Endomorphism.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/Endomorphism.h.dvi from ${IN}/Endomorphism.h.pamphlet 
	@${DOCUMENT} dvi Endomorphism.h ${DOC} Endomorphism.h ${NOISE}


${OUT}/MapEnum.h: ${IN}/MapEnum.h.pamphlet
	@echo 5 making ${OUT}/MapEnum.h from ${IN}/MapEnum.h.pamphlet
	@${DOCUMENT} src MapEnum.h ${OUT}

${DOC}/MapEnum.h.dvi: ${IN}/MapEnum.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/MapEnum.h.dvi from ${IN}/MapEnum.h.pamphlet 
	@${DOCUMENT} dvi MapEnum.h ${DOC} MapEnum.h ${NOISE}


${OUT}/Map.h: ${IN}/Map.h.pamphlet
	@echo 7 making ${OUT}/Map.h from ${IN}/Map.h.pamphlet
	@${DOCUMENT} src Map.h ${OUT}

${DOC}/Map.h.dvi: ${IN}/Map.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/Map.h.dvi from ${IN}/Map.h.pamphlet 
	@${DOCUMENT} dvi Map.h ${DOC} Map.h ${NOISE}


${OUT}/MapParser.h: ${IN}/MapParser.h.pamphlet
	@echo 9 making ${OUT}/MapParser.h from ${IN}/MapParser.h.pamphlet
	@${DOCUMENT} src MapParser.h ${OUT}

${DOC}/MapParser.h.dvi: ${IN}/MapParser.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/MapParser.h.dvi from ${IN}/MapParser.h.pamphlet 
	@${DOCUMENT} dvi MapParser.h ${DOC} MapParser.h ${NOISE}


${OUT}/RandomAutoInFree.h: ${IN}/RandomAutoInFree.h.pamphlet
	@echo 11 making ${OUT}/RandomAutoInFree.h from ${IN}/RandomAutoInFree.h.pamphlet
	@${DOCUMENT} src RandomAutoInFree.h ${OUT}

${DOC}/RandomAutoInFree.h.dvi: ${IN}/RandomAutoInFree.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/RandomAutoInFree.h.dvi from ${IN}/RandomAutoInFree.h.pamphlet 
	@${DOCUMENT} dvi RandomAutoInFree.h ${DOC} RandomAutoInFree.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 13 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
