IN=${SRC}/backend/Polynomial/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Polynomial/include


FILES= ${OUT}/BraidGroupToolkit.h ${OUT}/PBTree.h \
       ${OUT}/WPolynomial.h

DOCFILES= ${DOC}/BraidGroupToolkit.h.dvi ${DOC}/PBTree.h.dvi \
          ${DOC}/WPolynomial.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 78 finished ${IN}

${OUT}/BraidGroupToolkit.h: ${IN}/BraidGroupToolkit.h.pamphlet
	@echo 73 making ${OUT}/BraidGroupToolkit.h from ${IN}/BraidGroupToolkit.h.pamphlet
	@${DOCUMENT} src BraidGroupToolkit.h ${OUT}

${DOC}/BraidGroupToolkit.h.dvi: ${IN}/BraidGroupToolkit.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/BraidGroupToolkit.h.dvi from ${IN}/BraidGroupToolkit.h.pamphlet 
	@${DOCUMENT} dvi BraidGroupToolkit.h ${DOC} BraidGroupToolkit.h ${NOISE}


${OUT}/PBTree.h: ${IN}/PBTree.h.pamphlet
	@echo 73 making ${OUT}/PBTree.h from ${IN}/PBTree.h.pamphlet
	@${DOCUMENT} src PBTree.h ${OUT}

${DOC}/PBTree.h.dvi: ${IN}/PBTree.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/PBTree.h.dvi from ${IN}/PBTree.h.pamphlet 
	@${DOCUMENT} dvi PBTree.h ${DOC} PBTree.h ${NOISE}


${OUT}/WPolynomial.h: ${IN}/WPolynomial.h.pamphlet
	@echo 73 making ${OUT}/WPolynomial.h from ${IN}/WPolynomial.h.pamphlet
	@${DOCUMENT} src WPolynomial.h ${OUT}

${DOC}/WPolynomial.h.dvi: ${IN}/WPolynomial.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/WPolynomial.h.dvi from ${IN}/WPolynomial.h.pamphlet 
	@${DOCUMENT} dvi WPolynomial.h ${DOC} WPolynomial.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 77 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
