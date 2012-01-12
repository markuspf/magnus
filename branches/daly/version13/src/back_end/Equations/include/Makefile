IN=${SRC}/backend/Equations/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Equations/include


FILES= ${OUT}/DGESolver.h ${OUT}/NielsenTransformations.h \
       ${OUT}/QEqnSolutions.h ${OUT}/Queue.h \
       ${OUT}/SolutionsEnum.h ${OUT}/TupleEnumerator.h \
       ${OUT}/VectorPtr.h ${OUT}/VertexInfo.h

DOCFILES= ${DOC}/DGESolver.h.dvi ${DOC}/NielsenTransformations.h.dvi \
          ${DOC}/QEqnSolutions.h.dvi ${DOC}/Queue.h.dvi \
          ${DOC}/SolutionsEnum.h.dvi ${DOC}/TupleEnumerator.h.dvi \
          ${DOC}/VectorPtr.h.dvi ${DOC}/VertexInfo.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 18 finished ${IN}

${OUT}/DGESolver.h: ${IN}/DGESolver.h.pamphlet
	@echo 1 making ${OUT}/DGESolver.h from ${IN}/DGESolver.h.pamphlet
	@${DOCUMENT} src DGESolver.h ${OUT}

${DOC}/DGESolver.h.dvi: ${IN}/DGESolver.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/DGESolver.h.dvi from ${IN}/DGESolver.h.pamphlet 
	@${DOCUMENT} dvi DGESolver.h ${DOC} DGESolver.h ${NOISE}


${OUT}/NielsenTransformations.h: ${IN}/NielsenTransformations.h.pamphlet
	@echo 3 making ${OUT}/NielsenTransformations.h from ${IN}/NielsenTransformations.h.pamphlet
	@${DOCUMENT} src NielsenTransformations.h ${OUT}

${DOC}/NielsenTransformations.h.dvi: ${IN}/NielsenTransformations.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/NielsenTransformations.h.dvi from ${IN}/NielsenTransformations.h.pamphlet 
	@${DOCUMENT} dvi NielsenTransformations.h ${DOC} NielsenTransformations.h ${NOISE}


${OUT}/QEqnSolutions.h: ${IN}/QEqnSolutions.h.pamphlet
	@echo 5 making ${OUT}/QEqnSolutions.h from ${IN}/QEqnSolutions.h.pamphlet
	@${DOCUMENT} src QEqnSolutions.h ${OUT}

${DOC}/QEqnSolutions.h.dvi: ${IN}/QEqnSolutions.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/QEqnSolutions.h.dvi from ${IN}/QEqnSolutions.h.pamphlet 
	@${DOCUMENT} dvi QEqnSolutions.h ${DOC} QEqnSolutions.h ${NOISE}


${OUT}/Queue.h: ${IN}/Queue.h.pamphlet
	@echo 7 making ${OUT}/Queue.h from ${IN}/Queue.h.pamphlet
	@${DOCUMENT} src Queue.h ${OUT}

${DOC}/Queue.h.dvi: ${IN}/Queue.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/Queue.h.dvi from ${IN}/Queue.h.pamphlet 
	@${DOCUMENT} dvi Queue.h ${DOC} Queue.h ${NOISE}


${OUT}/SolutionsEnum.h: ${IN}/SolutionsEnum.h.pamphlet
	@echo 9 making ${OUT}/SolutionsEnum.h from ${IN}/SolutionsEnum.h.pamphlet
	@${DOCUMENT} src SolutionsEnum.h ${OUT}

${DOC}/SolutionsEnum.h.dvi: ${IN}/SolutionsEnum.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/SolutionsEnum.h.dvi from ${IN}/SolutionsEnum.h.pamphlet 
	@${DOCUMENT} dvi SolutionsEnum.h ${DOC} SolutionsEnum.h ${NOISE}


${OUT}/TupleEnumerator.h: ${IN}/TupleEnumerator.h.pamphlet
	@echo 11 making ${OUT}/TupleEnumerator.h from ${IN}/TupleEnumerator.h.pamphlet
	@${DOCUMENT} src TupleEnumerator.h ${OUT}

${DOC}/TupleEnumerator.h.dvi: ${IN}/TupleEnumerator.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/TupleEnumerator.h.dvi from ${IN}/TupleEnumerator.h.pamphlet 
	@${DOCUMENT} dvi TupleEnumerator.h ${DOC} TupleEnumerator.h ${NOISE}


${OUT}/VectorPtr.h: ${IN}/VectorPtr.h.pamphlet
	@echo 13 making ${OUT}/VectorPtr.h from ${IN}/VectorPtr.h.pamphlet
	@${DOCUMENT} src VectorPtr.h ${OUT}

${DOC}/VectorPtr.h.dvi: ${IN}/VectorPtr.h.pamphlet ${DOC}/axiom.sty
	@echo 14 making ${DOC}/VectorPtr.h.dvi from ${IN}/VectorPtr.h.pamphlet 
	@${DOCUMENT} dvi VectorPtr.h ${DOC} VectorPtr.h ${NOISE}


${OUT}/VertexInfo.h: ${IN}/VertexInfo.h.pamphlet
	@echo 15 making ${OUT}/VertexInfo.h from ${IN}/VertexInfo.h.pamphlet
	@${DOCUMENT} src VertexInfo.h ${OUT}

${DOC}/VertexInfo.h.dvi: ${IN}/VertexInfo.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/VertexInfo.h.dvi from ${IN}/VertexInfo.h.pamphlet 
	@${DOCUMENT} dvi VertexInfo.h ${DOC} VertexInfo.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 17 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
