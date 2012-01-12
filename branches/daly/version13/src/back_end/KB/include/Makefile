IN=${SRC}/backend/KB/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/KB/include


FILES= ${OUT}/DiffHistory.h ${OUT}/DiffHistoryRep.h \
       ${OUT}/DiffMachine.h ${OUT}/DiffMachineRep.h \
       ${OUT}/GenMult.h ${OUT}/GenMultRep.h \
       ${OUT}/KBMachine.h ${OUT}/KBMachineRep.h \
       ${OUT}/KBmagPackage.h ${OUT}/RKBPackage.h \
       ${OUT}/WordOrder.h ${OUT}/WordOrderRep.h

DOCFILES= ${DOC}/DiffHistory.h.dvi ${DOC}/DiffHistoryRep.h.dvi \
          ${DOC}/DiffMachine.h.dvi ${DOC}/DiffMachineRep.h.dvi \
          ${DOC}/GenMult.h.dvi ${DOC}/GenMultRep.h.dvi \
          ${DOC}/KBMachine.h.dvi ${DOC}/KBMachineRep.h.dvi \
          ${DOC}/KBmagPackage.h.dvi ${DOC}/RKBPackage.h.dvi \
          ${DOC}/WordOrder.h.dvi ${DOC}/WordOrderRep.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 26 finished ${IN}

${OUT}/DiffHistory.h: ${IN}/DiffHistory.h.pamphlet
	@echo 1 making ${OUT}/DiffHistory.h from ${IN}/DiffHistory.h.pamphlet
	@${DOCUMENT} src DiffHistory.h ${OUT}

${DOC}/DiffHistory.h.dvi: ${IN}/DiffHistory.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/DiffHistory.h.dvi from ${IN}/DiffHistory.h.pamphlet 
	@${DOCUMENT} dvi DiffHistory.h ${DOC} DiffHistory.h ${NOISE}


${OUT}/DiffHistoryRep.h: ${IN}/DiffHistoryRep.h.pamphlet
	@echo 3 making ${OUT}/DiffHistoryRep.h from ${IN}/DiffHistoryRep.h.pamphlet
	@${DOCUMENT} src DiffHistoryRep.h ${OUT}

${DOC}/DiffHistoryRep.h.dvi: ${IN}/DiffHistoryRep.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/DiffHistoryRep.h.dvi from ${IN}/DiffHistoryRep.h.pamphlet 
	@${DOCUMENT} dvi DiffHistoryRep.h ${DOC} DiffHistoryRep.h ${NOISE}


${OUT}/DiffMachine.h: ${IN}/DiffMachine.h.pamphlet
	@echo 5 making ${OUT}/DiffMachine.h from ${IN}/DiffMachine.h.pamphlet
	@${DOCUMENT} src DiffMachine.h ${OUT}

${DOC}/DiffMachine.h.dvi: ${IN}/DiffMachine.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/DiffMachine.h.dvi from ${IN}/DiffMachine.h.pamphlet 
	@${DOCUMENT} dvi DiffMachine.h ${DOC} DiffMachine.h ${NOISE}


${OUT}/DiffMachineRep.h: ${IN}/DiffMachineRep.h.pamphlet
	@echo 7 making ${OUT}/DiffMachineRep.h from ${IN}/DiffMachineRep.h.pamphlet
	@${DOCUMENT} src DiffMachineRep.h ${OUT}

${DOC}/DiffMachineRep.h.dvi: ${IN}/DiffMachineRep.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/DiffMachineRep.h.dvi from ${IN}/DiffMachineRep.h.pamphlet 
	@${DOCUMENT} dvi DiffMachineRep.h ${DOC} DiffMachineRep.h ${NOISE}


${OUT}/GenMult.h: ${IN}/GenMult.h.pamphlet
	@echo 9 making ${OUT}/GenMult.h from ${IN}/GenMult.h.pamphlet
	@${DOCUMENT} src GenMult.h ${OUT}

${DOC}/GenMult.h.dvi: ${IN}/GenMult.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/GenMult.h.dvi from ${IN}/GenMult.h.pamphlet 
	@${DOCUMENT} dvi GenMult.h ${DOC} GenMult.h ${NOISE}


${OUT}/GenMultRep.h: ${IN}/GenMultRep.h.pamphlet
	@echo 11 making ${OUT}/GenMultRep.h from ${IN}/GenMultRep.h.pamphlet
	@${DOCUMENT} src GenMultRep.h ${OUT}

${DOC}/GenMultRep.h.dvi: ${IN}/GenMultRep.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/GenMultRep.h.dvi from ${IN}/GenMultRep.h.pamphlet 
	@${DOCUMENT} dvi GenMultRep.h ${DOC} GenMultRep.h ${NOISE}


${OUT}/KBMachine.h: ${IN}/KBMachine.h.pamphlet
	@echo 13 making ${OUT}/KBMachine.h from ${IN}/KBMachine.h.pamphlet
	@${DOCUMENT} src KBMachine.h ${OUT}

${DOC}/KBMachine.h.dvi: ${IN}/KBMachine.h.pamphlet ${DOC}/axiom.sty
	@echo 14 making ${DOC}/KBMachine.h.dvi from ${IN}/KBMachine.h.pamphlet 
	@${DOCUMENT} dvi KBMachine.h ${DOC} KBMachine.h ${NOISE}


${OUT}/KBMachineRep.h: ${IN}/KBMachineRep.h.pamphlet
	@echo 15 making ${OUT}/KBMachineRep.h from ${IN}/KBMachineRep.h.pamphlet
	@${DOCUMENT} src KBMachineRep.h ${OUT}

${DOC}/KBMachineRep.h.dvi: ${IN}/KBMachineRep.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/KBMachineRep.h.dvi from ${IN}/KBMachineRep.h.pamphlet 
	@${DOCUMENT} dvi KBMachineRep.h ${DOC} KBMachineRep.h ${NOISE}


${OUT}/KBmagPackage.h: ${IN}/KBmagPackage.h.pamphlet
	@echo 17 making ${OUT}/KBmagPackage.h from ${IN}/KBmagPackage.h.pamphlet
	@${DOCUMENT} src KBmagPackage.h ${OUT}

${DOC}/KBmagPackage.h.dvi: ${IN}/KBmagPackage.h.pamphlet ${DOC}/axiom.sty
	@echo 18 making ${DOC}/KBmagPackage.h.dvi from ${IN}/KBmagPackage.h.pamphlet 
	@${DOCUMENT} dvi KBmagPackage.h ${DOC} KBmagPackage.h ${NOISE}


${OUT}/RKBPackage.h: ${IN}/RKBPackage.h.pamphlet
	@echo 19 making ${OUT}/RKBPackage.h from ${IN}/RKBPackage.h.pamphlet
	@${DOCUMENT} src RKBPackage.h ${OUT}

${DOC}/RKBPackage.h.dvi: ${IN}/RKBPackage.h.pamphlet ${DOC}/axiom.sty
	@echo 20 making ${DOC}/RKBPackage.h.dvi from ${IN}/RKBPackage.h.pamphlet 
	@${DOCUMENT} dvi RKBPackage.h ${DOC} RKBPackage.h ${NOISE}


${OUT}/WordOrder.h: ${IN}/WordOrder.h.pamphlet
	@echo 21 making ${OUT}/WordOrder.h from ${IN}/WordOrder.h.pamphlet
	@${DOCUMENT} src WordOrder.h ${OUT}

${DOC}/WordOrder.h.dvi: ${IN}/WordOrder.h.pamphlet ${DOC}/axiom.sty
	@echo 22 making ${DOC}/WordOrder.h.dvi from ${IN}/WordOrder.h.pamphlet 
	@${DOCUMENT} dvi WordOrder.h ${DOC} WordOrder.h ${NOISE}


${OUT}/WordOrderRep.h: ${IN}/WordOrderRep.h.pamphlet
	@echo 23 making ${OUT}/WordOrderRep.h from ${IN}/WordOrderRep.h.pamphlet
	@${DOCUMENT} src WordOrderRep.h ${OUT}

${DOC}/WordOrderRep.h.dvi: ${IN}/WordOrderRep.h.pamphlet ${DOC}/axiom.sty
	@echo 24 making ${DOC}/WordOrderRep.h.dvi from ${IN}/WordOrderRep.h.pamphlet 
	@${DOCUMENT} dvi WordOrderRep.h ${DOC} WordOrderRep.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 25 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
