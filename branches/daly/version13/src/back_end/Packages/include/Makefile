IN=${SRC}/backend/Packages/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Packages/include


FILES= ${OUT}/PackagesMessageParser.h ${OUT}/PackagesObject.h 

DOCFILES= ${DOC}/PackagesMessageParser.h.dvi ${DOC}/PackagesObject.h.dvi 

all: ${FILES} ${DOCFILES}
	@echo 78 finished ${IN}

${OUT}/PackagesMessageParser.h: ${IN}/PackagesMessageParser.h.pamphlet
	@echo 73 making ${OUT}/PackagesMessageParser.h from ${IN}/PackagesMessageParser.h.pamphlet
	@${DOCUMENT} src PackagesMessageParser.h ${OUT}

${DOC}/PackagesMessageParser.h.dvi: ${IN}/PackagesMessageParser.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/PackagesMessageParser.h.dvi from ${IN}/PackagesMessageParser.h.pamphlet 
	@${DOCUMENT} dvi PackagesMessageParser.h ${DOC} PackagesMessageParser.h ${NOISE}


${OUT}/PackagesObject.h: ${IN}/PackagesObject.h.pamphlet
	@echo 73 making ${OUT}/PackagesObject.h from ${IN}/PackagesObject.h.pamphlet
	@${DOCUMENT} src PackagesObject.h ${OUT}

${DOC}/PackagesObject.h.dvi: ${IN}/PackagesObject.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/PackagesObject.h.dvi from ${IN}/PackagesObject.h.pamphlet 
	@${DOCUMENT} dvi PackagesObject.h ${DOC} PackagesObject.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 77 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
