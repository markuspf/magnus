IN=${SRC}/backend/Matrix/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Matrix/include


FILES= ${OUT}/Char.h ${OUT}/GaussTransformation.h \
       ${OUT}/HomomorphismBuilder.h ${OUT}/MatrixComputations.h \
       ${OUT}/Matrix.h ${OUT}/Polynomial.h \
       ${OUT}/RandomMatrix.h ${OUT}/RingParser.h

DOCFILES= ${DOC}/Char.h.dvi ${DOC}/GaussTransformation.h.dvi \
          ${DOC}/HomomorphismBuilder.h.dvi ${DOC}/MatrixComputations.h.dvi \
          ${DOC}/Matrix.h.dvi ${DOC}/Polynomial.h.dvi \
          ${DOC}/RandomMatrix.h.dvi ${DOC}/RingParser.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 18 finished ${IN}

${OUT}/Char.h: ${IN}/Char.h.pamphlet
	@echo 1 making ${OUT}/Char.h from ${IN}/Char.h.pamphlet
	@${DOCUMENT} src Char.h ${OUT}

${DOC}/Char.h.dvi: ${IN}/Char.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/Char.h.dvi from ${IN}/Char.h.pamphlet 
	@${DOCUMENT} dvi Char.h ${DOC} Char.h ${NOISE}


${OUT}/GaussTransformation.h: ${IN}/GaussTransformation.h.pamphlet
	@echo 3 making ${OUT}/GaussTransformation.h from ${IN}/GaussTransformation.h.pamphlet
	@${DOCUMENT} src GaussTransformation.h ${OUT}

${DOC}/GaussTransformation.h.dvi: ${IN}/GaussTransformation.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/GaussTransformation.h.dvi from ${IN}/GaussTransformation.h.pamphlet 
	@${DOCUMENT} dvi GaussTransformation.h ${DOC} GaussTransformation.h ${NOISE}


${OUT}/HomomorphismBuilder.h: ${IN}/HomomorphismBuilder.h.pamphlet
	@echo 5 making ${OUT}/HomomorphismBuilder.h from ${IN}/HomomorphismBuilder.h.pamphlet
	@${DOCUMENT} src HomomorphismBuilder.h ${OUT}

${DOC}/HomomorphismBuilder.h.dvi: ${IN}/HomomorphismBuilder.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/HomomorphismBuilder.h.dvi from ${IN}/HomomorphismBuilder.h.pamphlet 
	@${DOCUMENT} dvi HomomorphismBuilder.h ${DOC} HomomorphismBuilder.h ${NOISE}


${OUT}/MatrixComputations.h: ${IN}/MatrixComputations.h.pamphlet
	@echo 7 making ${OUT}/MatrixComputations.h from ${IN}/MatrixComputations.h.pamphlet
	@${DOCUMENT} src MatrixComputations.h ${OUT}

${DOC}/MatrixComputations.h.dvi: ${IN}/MatrixComputations.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/MatrixComputations.h.dvi from ${IN}/MatrixComputations.h.pamphlet 
	@${DOCUMENT} dvi MatrixComputations.h ${DOC} MatrixComputations.h ${NOISE}


${OUT}/Matrix.h: ${IN}/Matrix.h.pamphlet
	@echo 9 making ${OUT}/Matrix.h from ${IN}/Matrix.h.pamphlet
	@${DOCUMENT} src Matrix.h ${OUT}

${DOC}/Matrix.h.dvi: ${IN}/Matrix.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/Matrix.h.dvi from ${IN}/Matrix.h.pamphlet 
	@${DOCUMENT} dvi Matrix.h ${DOC} Matrix.h ${NOISE}


${OUT}/Polynomial.h: ${IN}/Polynomial.h.pamphlet
	@echo 11 making ${OUT}/Polynomial.h from ${IN}/Polynomial.h.pamphlet
	@${DOCUMENT} src Polynomial.h ${OUT}

${DOC}/Polynomial.h.dvi: ${IN}/Polynomial.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/Polynomial.h.dvi from ${IN}/Polynomial.h.pamphlet 
	@${DOCUMENT} dvi Polynomial.h ${DOC} Polynomial.h ${NOISE}


${OUT}/RandomMatrix.h: ${IN}/RandomMatrix.h.pamphlet
	@echo 13 making ${OUT}/RandomMatrix.h from ${IN}/RandomMatrix.h.pamphlet
	@${DOCUMENT} src RandomMatrix.h ${OUT}

${DOC}/RandomMatrix.h.dvi: ${IN}/RandomMatrix.h.pamphlet ${DOC}/axiom.sty
	@echo 14 making ${DOC}/RandomMatrix.h.dvi from ${IN}/RandomMatrix.h.pamphlet 
	@${DOCUMENT} dvi RandomMatrix.h ${DOC} RandomMatrix.h ${NOISE}


${OUT}/RingParser.h: ${IN}/RingParser.h.pamphlet
	@echo 15 making ${OUT}/RingParser.h from ${IN}/RingParser.h.pamphlet
	@${DOCUMENT} src RingParser.h ${OUT}

${DOC}/RingParser.h.dvi: ${IN}/RingParser.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/RingParser.h.dvi from ${IN}/RingParser.h.pamphlet 
	@${DOCUMENT} dvi RingParser.h ${DOC} RingParser.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 17 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
