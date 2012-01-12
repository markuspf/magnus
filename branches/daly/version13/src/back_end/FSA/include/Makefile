IN=${SRC}/backend/FSA/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/FSA/include


FILES= ${OUT}/DFSA.h ${OUT}/DFSAParser.h ${OUT}/DFSARep.h \
       ${OUT}/FSA.h ${OUT}/FSARep.h ${OUT}/StatePair.h

DOCFILES= ${DOC}/DFSA.h.dvi ${DOC}/DFSAParser.h.dvi ${DOC}/DFSARep.h.dvi \
          ${DOC}/FSA.h.dvi ${DOC}/FSARep.h.dvi ${DOC}/StatePair.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 14 finished ${IN}

${OUT}/DFSA.h: ${IN}/DFSA.h.pamphlet
	@echo 1 making ${OUT}/DFSA.h from ${IN}/DFSA.h.pamphlet
	@${DOCUMENT} src DFSA.h ${OUT}

${DOC}/DFSA.h.dvi: ${IN}/DFSA.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/DFSA.h.dvi from ${IN}/DFSA.h.pamphlet 
	@${DOCUMENT} dvi DFSA.h ${DOC} DFSA.h ${NOISE}


${OUT}/DFSAParser.h: ${IN}/DFSAParser.h.pamphlet
	@echo 3 making ${OUT}/DFSAParser.h from ${IN}/DFSAParser.h.pamphlet
	@${DOCUMENT} src DFSAParser.h ${OUT}

${DOC}/DFSAParser.h.dvi: ${IN}/DFSAParser.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/DFSAParser.h.dvi from ${IN}/DFSAParser.h.pamphlet 
	@${DOCUMENT} dvi DFSAParser.h ${DOC} DFSAParser.h ${NOISE}


${OUT}/DFSARep.h: ${IN}/DFSARep.h.pamphlet
	@echo 5 making ${OUT}/DFSARep.h from ${IN}/DFSARep.h.pamphlet
	@${DOCUMENT} src DFSARep.h ${OUT}

${DOC}/DFSARep.h.dvi: ${IN}/DFSARep.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/DFSARep.h.dvi from ${IN}/DFSARep.h.pamphlet 
	@${DOCUMENT} dvi DFSARep.h ${DOC} DFSARep.h ${NOISE}


${OUT}/FSA.h: ${IN}/FSA.h.pamphlet
	@echo 7 making ${OUT}/FSA.h from ${IN}/FSA.h.pamphlet
	@${DOCUMENT} src FSA.h ${OUT}

${DOC}/FSA.h.dvi: ${IN}/FSA.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/FSA.h.dvi from ${IN}/FSA.h.pamphlet 
	@${DOCUMENT} dvi FSA.h ${DOC} FSA.h ${NOISE}


${OUT}/FSARep.h: ${IN}/FSARep.h.pamphlet
	@echo 9 making ${OUT}/FSARep.h from ${IN}/FSARep.h.pamphlet
	@${DOCUMENT} src FSARep.h ${OUT}

${DOC}/FSARep.h.dvi: ${IN}/FSARep.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/FSARep.h.dvi from ${IN}/FSARep.h.pamphlet 
	@${DOCUMENT} dvi FSARep.h ${DOC} FSARep.h ${NOISE}


${OUT}/StatePair.h: ${IN}/StatePair.h.pamphlet
	@echo 11 making ${OUT}/StatePair.h from ${IN}/StatePair.h.pamphlet
	@${DOCUMENT} src StatePair.h ${OUT}

${DOC}/StatePair.h.dvi: ${IN}/StatePair.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/StatePair.h.dvi from ${IN}/StatePair.h.pamphlet 
	@${DOCUMENT} dvi StatePair.h ${DOC} StatePair.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 13 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
