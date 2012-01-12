IN=${SRC}/backend/GAP/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/GAP/include


FILES= ${OUT}/GAP.h ${OUT}/GAPManager.h ${OUT}/Permutation.h \
       ${OUT}/PermutationParser.h

DOCFILES= ${DOC}/GAP.h.dvi ${DOC}/GAPManager.h.dvi ${DOC}/Permutation.h.dvi \
          ${DOC}/PermutationParser.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 78 finished ${IN}

${OUT}/GAP.h: ${IN}/GAP.h.pamphlet
	@echo 73 making ${OUT}/GAP.h from ${IN}/GAP.h.pamphlet
	@${DOCUMENT} src GAP.h ${OUT}

${DOC}/GAP.h.dvi: ${IN}/GAP.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/GAP.h.dvi from ${IN}/GAP.h.pamphlet 
	@${DOCUMENT} dvi GAP.h ${DOC} GAP.h ${NOISE}


${OUT}/GAPManager.h: ${IN}/GAPManager.h.pamphlet
	@echo 73 making ${OUT}/GAPManager.h from ${IN}/GAPManager.h.pamphlet
	@${DOCUMENT} src GAPManager.h ${OUT}

${DOC}/GAPManager.h.dvi: ${IN}/GAPManager.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/GAPManager.h.dvi from ${IN}/GAPManager.h.pamphlet 
	@${DOCUMENT} dvi GAPManager.h ${DOC} GAPManager.h ${NOISE}


${OUT}/Permutation.h: ${IN}/Permutation.h.pamphlet
	@echo 73 making ${OUT}/Permutation.h from ${IN}/Permutation.h.pamphlet
	@${DOCUMENT} src Permutation.h ${OUT}

${DOC}/Permutation.h.dvi: ${IN}/Permutation.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/Permutation.h.dvi from ${IN}/Permutation.h.pamphlet 
	@${DOCUMENT} dvi Permutation.h ${DOC} Permutation.h ${NOISE}


${OUT}/PermutationParser.h: ${IN}/PermutationParser.h.pamphlet
	@echo 73 making ${OUT}/PermutationParser.h from ${IN}/PermutationParser.h.pamphlet
	@${DOCUMENT} src PermutationParser.h ${OUT}

${DOC}/PermutationParser.h.dvi: ${IN}/PermutationParser.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/PermutationParser.h.dvi from ${IN}/PermutationParser.h.pamphlet 
	@${DOCUMENT} dvi PermutationParser.h ${DOC} PermutationParser.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 77 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
