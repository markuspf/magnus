IN=${SRC}/backend/Group/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/Group/include


FILES= ${OUT}/AbelianEquations.h ${OUT}/AbelianGroup.h \
       ${OUT}/AbelianGroupRep.h \
       ${OUT}/AbelianInfinitenessProblem.h \
       ${OUT}/AbelianSGPresentation.h ${OUT}/EqSystemParser.h \
       ${OUT}/EquationParser.h ${OUT}/FGGroup.h \
       ${OUT}/FGGroupRep.h ${OUT}/FPGroup.h \
       ${OUT}/FPGroupRep.h ${OUT}/FreeByCyclic.h \
       ${OUT}/FreeGroup.h ${OUT}/FreeGroupRep.h \
       ${OUT}/GeneralWhitehead.h ${OUT}/GroupFastChecks.h \
       ${OUT}/Group.h ${OUT}/GroupRep.h ${OUT}/Homology.h \
       ${OUT}/MSCGConjugacyProblem.h ${OUT}/MSCGroup.h \
       ${OUT}/ORWordProblem.h ${OUT}/PowerSeriesWP.h \
       ${OUT}/PresentationParser.h ${OUT}/PrimeNumbers.h \
       ${OUT}/Products.h ${OUT}/RandomAutomorphism.h \
       ${OUT}/RandomMSCGroup.h ${OUT}/RandomPrimitiveElement.h \
       ${OUT}/RipsConstruction.h ${OUT}/RSApproximation.h \
       ${OUT}/RSWord.h ${OUT}/ShortenByRelators.h \
       ${OUT}/SmithNormalForm.h ${OUT}/SymmetricRelators.h \
       ${OUT}/TietzeTrekker.h ${OUT}/TTP.h \
       ${OUT}/WordEnumerator.h

DOCFILES= ${DOC}/AbelianEquations.h.dvi ${DOC}/AbelianGroup.h.dvi \
          ${DOC}/AbelianGroupRep.h.dvi \
          ${DOC}/AbelianInfinitenessProblem.h.dvi \
          ${DOC}/AbelianSGPresentation.h.dvi ${DOC}/EqSystemParser.h.dvi \
          ${DOC}/EquationParser.h.dvi ${DOC}/FGGroup.h.dvi \
          ${DOC}/FGGroupRep.h.dvi ${DOC}/FPGroup.h.dvi \
          ${DOC}/FPGroupRep.h.dvi ${DOC}/FreeByCyclic.h.dvi \
          ${DOC}/FreeGroup.h.dvi ${DOC}/FreeGroupRep.h.dvi \
          ${DOC}/GeneralWhitehead.h.dvi ${DOC}/GroupFastChecks.h.dvi \
          ${DOC}/Group.h.dvi ${DOC}/GroupRep.h.dvi ${DOC}/Homology.h.dvi \
          ${DOC}/MSCGConjugacyProblem.h.dvi ${DOC}/MSCGroup.h.dvi \
          ${DOC}/ORWordProblem.h.dvi ${DOC}/PowerSeriesWP.h.dvi \
          ${DOC}/PresentationParser.h.dvi ${DOC}/PrimeNumbers.h.dvi \
          ${DOC}/Products.h.dvi ${DOC}/RandomAutomorphism.h.dvi \
          ${DOC}/RandomMSCGroup.h.dvi ${DOC}/RandomPrimitiveElement.h.dvi \
          ${DOC}/RipsConstruction.h.dvi ${DOC}/RSApproximation.h.dvi \
          ${DOC}/RSWord.h.dvi ${DOC}/ShortenByRelators.h.dvi \
          ${DOC}/SmithNormalForm.h.dvi ${DOC}/SymmetricRelators.h.dvi \
          ${DOC}/TietzeTrekker.h.dvi ${DOC}/TTP.h.dvi \
          ${DOC}/WordEnumerator.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 78 finished ${IN}

${OUT}/AbelianEquations.h: ${IN}/AbelianEquations.h.pamphlet
	@echo 1 making ${OUT}/AbelianEquations.h from ${IN}/AbelianEquations.h.pamphlet
	@${DOCUMENT} src AbelianEquations.h ${OUT}

${DOC}/AbelianEquations.h.dvi: ${IN}/AbelianEquations.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/AbelianEquations.h.dvi from ${IN}/AbelianEquations.h.pamphlet 
	@${DOCUMENT} dvi AbelianEquations.h ${DOC} AbelianEquations.h ${NOISE}


${OUT}/AbelianGroup.h: ${IN}/AbelianGroup.h.pamphlet
	@echo 3 making ${OUT}/AbelianGroup.h from ${IN}/AbelianGroup.h.pamphlet
	@${DOCUMENT} src AbelianGroup.h ${OUT}

${DOC}/AbelianGroup.h.dvi: ${IN}/AbelianGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/AbelianGroup.h.dvi from ${IN}/AbelianGroup.h.pamphlet 
	@${DOCUMENT} dvi AbelianGroup.h ${DOC} AbelianGroup.h ${NOISE}


${OUT}/AbelianGroupRep.h: ${IN}/AbelianGroupRep.h.pamphlet
	@echo 5 making ${OUT}/AbelianGroupRep.h from ${IN}/AbelianGroupRep.h.pamphlet
	@${DOCUMENT} src AbelianGroupRep.h ${OUT}

${DOC}/AbelianGroupRep.h.dvi: ${IN}/AbelianGroupRep.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/AbelianGroupRep.h.dvi from ${IN}/AbelianGroupRep.h.pamphlet 
	@${DOCUMENT} dvi AbelianGroupRep.h ${DOC} AbelianGroupRep.h ${NOISE}


${OUT}/AbelianInfinitenessProblem.h: ${IN}/AbelianInfinitenessProblem.h.pamphlet
	@echo 7 making ${OUT}/AbelianInfinitenessProblem.h from ${IN}/AbelianInfinitenessProblem.h.pamphlet
	@${DOCUMENT} src AbelianInfinitenessProblem.h ${OUT}

${DOC}/AbelianInfinitenessProblem.h.dvi: ${IN}/AbelianInfinitenessProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/AbelianInfinitenessProblem.h.dvi from ${IN}/AbelianInfinitenessProblem.h.pamphlet 
	@${DOCUMENT} dvi AbelianInfinitenessProblem.h ${DOC} AbelianInfinitenessProblem.h ${NOISE}


${OUT}/AbelianSGPresentation.h: ${IN}/AbelianSGPresentation.h.pamphlet
	@echo 9 making ${OUT}/AbelianSGPresentation.h from ${IN}/AbelianSGPresentation.h.pamphlet
	@${DOCUMENT} src AbelianSGPresentation.h ${OUT}

${DOC}/AbelianSGPresentation.h.dvi: ${IN}/AbelianSGPresentation.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/AbelianSGPresentation.h.dvi from ${IN}/AbelianSGPresentation.h.pamphlet 
	@${DOCUMENT} dvi AbelianSGPresentation.h ${DOC} AbelianSGPresentation.h ${NOISE}


${OUT}/EqSystemParser.h: ${IN}/EqSystemParser.h.pamphlet
	@echo 11 making ${OUT}/EqSystemParser.h from ${IN}/EqSystemParser.h.pamphlet
	@${DOCUMENT} src EqSystemParser.h ${OUT}

${DOC}/EqSystemParser.h.dvi: ${IN}/EqSystemParser.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/EqSystemParser.h.dvi from ${IN}/EqSystemParser.h.pamphlet 
	@${DOCUMENT} dvi EqSystemParser.h ${DOC} EqSystemParser.h ${NOISE}


${OUT}/EquationParser.h: ${IN}/EquationParser.h.pamphlet
	@echo 13 making ${OUT}/EquationParser.h from ${IN}/EquationParser.h.pamphlet
	@${DOCUMENT} src EquationParser.h ${OUT}

${DOC}/EquationParser.h.dvi: ${IN}/EquationParser.h.pamphlet ${DOC}/axiom.sty
	@echo 14 making ${DOC}/EquationParser.h.dvi from ${IN}/EquationParser.h.pamphlet 
	@${DOCUMENT} dvi EquationParser.h ${DOC} EquationParser.h ${NOISE}


${OUT}/FGGroup.h: ${IN}/FGGroup.h.pamphlet
	@echo 15 making ${OUT}/FGGroup.h from ${IN}/FGGroup.h.pamphlet
	@${DOCUMENT} src FGGroup.h ${OUT}

${DOC}/FGGroup.h.dvi: ${IN}/FGGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/FGGroup.h.dvi from ${IN}/FGGroup.h.pamphlet 
	@${DOCUMENT} dvi FGGroup.h ${DOC} FGGroup.h ${NOISE}


${OUT}/FGGroupRep.h: ${IN}/FGGroupRep.h.pamphlet
	@echo 17 making ${OUT}/FGGroupRep.h from ${IN}/FGGroupRep.h.pamphlet
	@${DOCUMENT} src FGGroupRep.h ${OUT}

${DOC}/FGGroupRep.h.dvi: ${IN}/FGGroupRep.h.pamphlet ${DOC}/axiom.sty
	@echo 18 making ${DOC}/FGGroupRep.h.dvi from ${IN}/FGGroupRep.h.pamphlet 
	@${DOCUMENT} dvi FGGroupRep.h ${DOC} FGGroupRep.h ${NOISE}


${OUT}/FPGroup.h: ${IN}/FPGroup.h.pamphlet
	@echo 19 making ${OUT}/FPGroup.h from ${IN}/FPGroup.h.pamphlet
	@${DOCUMENT} src FPGroup.h ${OUT}

${DOC}/FPGroup.h.dvi: ${IN}/FPGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 20 making ${DOC}/FPGroup.h.dvi from ${IN}/FPGroup.h.pamphlet 
	@${DOCUMENT} dvi FPGroup.h ${DOC} FPGroup.h ${NOISE}


${OUT}/FPGroupRep.h: ${IN}/FPGroupRep.h.pamphlet
	@echo 21 making ${OUT}/FPGroupRep.h from ${IN}/FPGroupRep.h.pamphlet
	@${DOCUMENT} src FPGroupRep.h ${OUT}

${DOC}/FPGroupRep.h.dvi: ${IN}/FPGroupRep.h.pamphlet ${DOC}/axiom.sty
	@echo 22 making ${DOC}/FPGroupRep.h.dvi from ${IN}/FPGroupRep.h.pamphlet 
	@${DOCUMENT} dvi FPGroupRep.h ${DOC} FPGroupRep.h ${NOISE}


${OUT}/FreeByCyclic.h: ${IN}/FreeByCyclic.h.pamphlet
	@echo 23 making ${OUT}/FreeByCyclic.h from ${IN}/FreeByCyclic.h.pamphlet
	@${DOCUMENT} src FreeByCyclic.h ${OUT}

${DOC}/FreeByCyclic.h.dvi: ${IN}/FreeByCyclic.h.pamphlet ${DOC}/axiom.sty
	@echo 24 making ${DOC}/FreeByCyclic.h.dvi from ${IN}/FreeByCyclic.h.pamphlet 
	@${DOCUMENT} dvi FreeByCyclic.h ${DOC} FreeByCyclic.h ${NOISE}


${OUT}/FreeGroup.h: ${IN}/FreeGroup.h.pamphlet
	@echo 25 making ${OUT}/FreeGroup.h from ${IN}/FreeGroup.h.pamphlet
	@${DOCUMENT} src FreeGroup.h ${OUT}

${DOC}/FreeGroup.h.dvi: ${IN}/FreeGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 26 making ${DOC}/FreeGroup.h.dvi from ${IN}/FreeGroup.h.pamphlet 
	@${DOCUMENT} dvi FreeGroup.h ${DOC} FreeGroup.h ${NOISE}


${OUT}/FreeGroupRep.h: ${IN}/FreeGroupRep.h.pamphlet
	@echo 27 making ${OUT}/FreeGroupRep.h from ${IN}/FreeGroupRep.h.pamphlet
	@${DOCUMENT} src FreeGroupRep.h ${OUT}

${DOC}/FreeGroupRep.h.dvi: ${IN}/FreeGroupRep.h.pamphlet ${DOC}/axiom.sty
	@echo 28 making ${DOC}/FreeGroupRep.h.dvi from ${IN}/FreeGroupRep.h.pamphlet 
	@${DOCUMENT} dvi FreeGroupRep.h ${DOC} FreeGroupRep.h ${NOISE}


${OUT}/GeneralWhitehead.h: ${IN}/GeneralWhitehead.h.pamphlet
	@echo 29 making ${OUT}/GeneralWhitehead.h from ${IN}/GeneralWhitehead.h.pamphlet
	@${DOCUMENT} src GeneralWhitehead.h ${OUT}

${DOC}/GeneralWhitehead.h.dvi: ${IN}/GeneralWhitehead.h.pamphlet ${DOC}/axiom.sty
	@echo 30 making ${DOC}/GeneralWhitehead.h.dvi from ${IN}/GeneralWhitehead.h.pamphlet 
	@${DOCUMENT} dvi GeneralWhitehead.h ${DOC} GeneralWhitehead.h ${NOISE}


${OUT}/GroupFastChecks.h: ${IN}/GroupFastChecks.h.pamphlet
	@echo 31 making ${OUT}/GroupFastChecks.h from ${IN}/GroupFastChecks.h.pamphlet
	@${DOCUMENT} src GroupFastChecks.h ${OUT}

${DOC}/GroupFastChecks.h.dvi: ${IN}/GroupFastChecks.h.pamphlet ${DOC}/axiom.sty
	@echo 32 making ${DOC}/GroupFastChecks.h.dvi from ${IN}/GroupFastChecks.h.pamphlet 
	@${DOCUMENT} dvi GroupFastChecks.h ${DOC} GroupFastChecks.h ${NOISE}


${OUT}/Group.h: ${IN}/Group.h.pamphlet
	@echo 33 making ${OUT}/Group.h from ${IN}/Group.h.pamphlet
	@${DOCUMENT} src Group.h ${OUT}

${DOC}/Group.h.dvi: ${IN}/Group.h.pamphlet ${DOC}/axiom.sty
	@echo 34 making ${DOC}/Group.h.dvi from ${IN}/Group.h.pamphlet 
	@${DOCUMENT} dvi Group.h ${DOC} Group.h ${NOISE}


${OUT}/GroupRep.h: ${IN}/GroupRep.h.pamphlet
	@echo 35 making ${OUT}/GroupRep.h from ${IN}/GroupRep.h.pamphlet
	@${DOCUMENT} src GroupRep.h ${OUT}

${DOC}/GroupRep.h.dvi: ${IN}/GroupRep.h.pamphlet ${DOC}/axiom.sty
	@echo 36 making ${DOC}/GroupRep.h.dvi from ${IN}/GroupRep.h.pamphlet 
	@${DOCUMENT} dvi GroupRep.h ${DOC} GroupRep.h ${NOISE}


${OUT}/Homology.h: ${IN}/Homology.h.pamphlet
	@echo 37 making ${OUT}/Homology.h from ${IN}/Homology.h.pamphlet
	@${DOCUMENT} src Homology.h ${OUT}

${DOC}/Homology.h.dvi: ${IN}/Homology.h.pamphlet ${DOC}/axiom.sty
	@echo 38 making ${DOC}/Homology.h.dvi from ${IN}/Homology.h.pamphlet 
	@${DOCUMENT} dvi Homology.h ${DOC} Homology.h ${NOISE}


${OUT}/MSCGConjugacyProblem.h: ${IN}/MSCGConjugacyProblem.h.pamphlet
	@echo 39 making ${OUT}/MSCGConjugacyProblem.h from ${IN}/MSCGConjugacyProblem.h.pamphlet
	@${DOCUMENT} src MSCGConjugacyProblem.h ${OUT}

${DOC}/MSCGConjugacyProblem.h.dvi: ${IN}/MSCGConjugacyProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 40 making ${DOC}/MSCGConjugacyProblem.h.dvi from ${IN}/MSCGConjugacyProblem.h.pamphlet 
	@${DOCUMENT} dvi MSCGConjugacyProblem.h ${DOC} MSCGConjugacyProblem.h ${NOISE}


${OUT}/MSCGroup.h: ${IN}/MSCGroup.h.pamphlet
	@echo 41 making ${OUT}/MSCGroup.h from ${IN}/MSCGroup.h.pamphlet
	@${DOCUMENT} src MSCGroup.h ${OUT}

${DOC}/MSCGroup.h.dvi: ${IN}/MSCGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 42 making ${DOC}/MSCGroup.h.dvi from ${IN}/MSCGroup.h.pamphlet 
	@${DOCUMENT} dvi MSCGroup.h ${DOC} MSCGroup.h ${NOISE}


${OUT}/ORWordProblem.h: ${IN}/ORWordProblem.h.pamphlet
	@echo 43 making ${OUT}/ORWordProblem.h from ${IN}/ORWordProblem.h.pamphlet
	@${DOCUMENT} src ORWordProblem.h ${OUT}

${DOC}/ORWordProblem.h.dvi: ${IN}/ORWordProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 44 making ${DOC}/ORWordProblem.h.dvi from ${IN}/ORWordProblem.h.pamphlet 
	@${DOCUMENT} dvi ORWordProblem.h ${DOC} ORWordProblem.h ${NOISE}


${OUT}/PowerSeriesWP.h: ${IN}/PowerSeriesWP.h.pamphlet
	@echo 45 making ${OUT}/PowerSeriesWP.h from ${IN}/PowerSeriesWP.h.pamphlet
	@${DOCUMENT} src PowerSeriesWP.h ${OUT}

${DOC}/PowerSeriesWP.h.dvi: ${IN}/PowerSeriesWP.h.pamphlet ${DOC}/axiom.sty
	@echo 46 making ${DOC}/PowerSeriesWP.h.dvi from ${IN}/PowerSeriesWP.h.pamphlet 
	@${DOCUMENT} dvi PowerSeriesWP.h ${DOC} PowerSeriesWP.h ${NOISE}


${OUT}/PresentationParser.h: ${IN}/PresentationParser.h.pamphlet
	@echo 47 making ${OUT}/PresentationParser.h from ${IN}/PresentationParser.h.pamphlet
	@${DOCUMENT} src PresentationParser.h ${OUT}

${DOC}/PresentationParser.h.dvi: ${IN}/PresentationParser.h.pamphlet ${DOC}/axiom.sty
	@echo 48 making ${DOC}/PresentationParser.h.dvi from ${IN}/PresentationParser.h.pamphlet 
	@${DOCUMENT} dvi PresentationParser.h ${DOC} PresentationParser.h ${NOISE}


${OUT}/PrimeNumbers.h: ${IN}/PrimeNumbers.h.pamphlet
	@echo 49 making ${OUT}/PrimeNumbers.h from ${IN}/PrimeNumbers.h.pamphlet
	@${DOCUMENT} src PrimeNumbers.h ${OUT}

${DOC}/PrimeNumbers.h.dvi: ${IN}/PrimeNumbers.h.pamphlet ${DOC}/axiom.sty
	@echo 50 making ${DOC}/PrimeNumbers.h.dvi from ${IN}/PrimeNumbers.h.pamphlet 
	@${DOCUMENT} dvi PrimeNumbers.h ${DOC} PrimeNumbers.h ${NOISE}


${OUT}/Products.h: ${IN}/Products.h.pamphlet
	@echo 51 making ${OUT}/Products.h from ${IN}/Products.h.pamphlet
	@${DOCUMENT} src Products.h ${OUT}

${DOC}/Products.h.dvi: ${IN}/Products.h.pamphlet ${DOC}/axiom.sty
	@echo 52 making ${DOC}/Products.h.dvi from ${IN}/Products.h.pamphlet 
	@${DOCUMENT} dvi Products.h ${DOC} Products.h ${NOISE}


${OUT}/RandomAutomorphism.h: ${IN}/RandomAutomorphism.h.pamphlet
	@echo 53 making ${OUT}/RandomAutomorphism.h from ${IN}/RandomAutomorphism.h.pamphlet
	@${DOCUMENT} src RandomAutomorphism.h ${OUT}

${DOC}/RandomAutomorphism.h.dvi: ${IN}/RandomAutomorphism.h.pamphlet ${DOC}/axiom.sty
	@echo 54 making ${DOC}/RandomAutomorphism.h.dvi from ${IN}/RandomAutomorphism.h.pamphlet 
	@${DOCUMENT} dvi RandomAutomorphism.h ${DOC} RandomAutomorphism.h ${NOISE}


${OUT}/RandomMSCGroup.h: ${IN}/RandomMSCGroup.h.pamphlet
	@echo 55 making ${OUT}/RandomMSCGroup.h from ${IN}/RandomMSCGroup.h.pamphlet
	@${DOCUMENT} src RandomMSCGroup.h ${OUT}

${DOC}/RandomMSCGroup.h.dvi: ${IN}/RandomMSCGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 56 making ${DOC}/RandomMSCGroup.h.dvi from ${IN}/RandomMSCGroup.h.pamphlet 
	@${DOCUMENT} dvi RandomMSCGroup.h ${DOC} RandomMSCGroup.h ${NOISE}


${OUT}/RandomPrimitiveElement.h: ${IN}/RandomPrimitiveElement.h.pamphlet
	@echo 57 making ${OUT}/RandomPrimitiveElement.h from ${IN}/RandomPrimitiveElement.h.pamphlet
	@${DOCUMENT} src RandomPrimitiveElement.h ${OUT}

${DOC}/RandomPrimitiveElement.h.dvi: ${IN}/RandomPrimitiveElement.h.pamphlet ${DOC}/axiom.sty
	@echo 58 making ${DOC}/RandomPrimitiveElement.h.dvi from ${IN}/RandomPrimitiveElement.h.pamphlet 
	@${DOCUMENT} dvi RandomPrimitiveElement.h ${DOC} RandomPrimitiveElement.h ${NOISE}


${OUT}/RipsConstruction.h: ${IN}/RipsConstruction.h.pamphlet
	@echo 59 making ${OUT}/RipsConstruction.h from ${IN}/RipsConstruction.h.pamphlet
	@${DOCUMENT} src RipsConstruction.h ${OUT}

${DOC}/RipsConstruction.h.dvi: ${IN}/RipsConstruction.h.pamphlet ${DOC}/axiom.sty
	@echo 60 making ${DOC}/RipsConstruction.h.dvi from ${IN}/RipsConstruction.h.pamphlet 
	@${DOCUMENT} dvi RipsConstruction.h ${DOC} RipsConstruction.h ${NOISE}


${OUT}/RSApproximation.h: ${IN}/RSApproximation.h.pamphlet
	@echo 61 making ${OUT}/RSApproximation.h from ${IN}/RSApproximation.h.pamphlet
	@${DOCUMENT} src RSApproximation.h ${OUT}

${DOC}/RSApproximation.h.dvi: ${IN}/RSApproximation.h.pamphlet ${DOC}/axiom.sty
	@echo 62 making ${DOC}/RSApproximation.h.dvi from ${IN}/RSApproximation.h.pamphlet 
	@${DOCUMENT} dvi RSApproximation.h ${DOC} RSApproximation.h ${NOISE}


${OUT}/RSWord.h: ${IN}/RSWord.h.pamphlet
	@echo 63 making ${OUT}/RSWord.h from ${IN}/RSWord.h.pamphlet
	@${DOCUMENT} src RSWord.h ${OUT}

${DOC}/RSWord.h.dvi: ${IN}/RSWord.h.pamphlet ${DOC}/axiom.sty
	@echo 64 making ${DOC}/RSWord.h.dvi from ${IN}/RSWord.h.pamphlet 
	@${DOCUMENT} dvi RSWord.h ${DOC} RSWord.h ${NOISE}


${OUT}/ShortenByRelators.h: ${IN}/ShortenByRelators.h.pamphlet
	@echo 65 making ${OUT}/ShortenByRelators.h from ${IN}/ShortenByRelators.h.pamphlet
	@${DOCUMENT} src ShortenByRelators.h ${OUT}

${DOC}/ShortenByRelators.h.dvi: ${IN}/ShortenByRelators.h.pamphlet ${DOC}/axiom.sty
	@echo 66 making ${DOC}/ShortenByRelators.h.dvi from ${IN}/ShortenByRelators.h.pamphlet 
	@${DOCUMENT} dvi ShortenByRelators.h ${DOC} ShortenByRelators.h ${NOISE}


${OUT}/SmithNormalForm.h: ${IN}/SmithNormalForm.h.pamphlet
	@echo 67 making ${OUT}/SmithNormalForm.h from ${IN}/SmithNormalForm.h.pamphlet
	@${DOCUMENT} src SmithNormalForm.h ${OUT}

${DOC}/SmithNormalForm.h.dvi: ${IN}/SmithNormalForm.h.pamphlet ${DOC}/axiom.sty
	@echo 68 making ${DOC}/SmithNormalForm.h.dvi from ${IN}/SmithNormalForm.h.pamphlet 
	@${DOCUMENT} dvi SmithNormalForm.h ${DOC} SmithNormalForm.h ${NOISE}


${OUT}/SymmetricRelators.h: ${IN}/SymmetricRelators.h.pamphlet
	@echo 69 making ${OUT}/SymmetricRelators.h from ${IN}/SymmetricRelators.h.pamphlet
	@${DOCUMENT} src SymmetricRelators.h ${OUT}

${DOC}/SymmetricRelators.h.dvi: ${IN}/SymmetricRelators.h.pamphlet ${DOC}/axiom.sty
	@echo 70 making ${DOC}/SymmetricRelators.h.dvi from ${IN}/SymmetricRelators.h.pamphlet 
	@${DOCUMENT} dvi SymmetricRelators.h ${DOC} SymmetricRelators.h ${NOISE}


${OUT}/TietzeTrekker.h: ${IN}/TietzeTrekker.h.pamphlet
	@echo 71 making ${OUT}/TietzeTrekker.h from ${IN}/TietzeTrekker.h.pamphlet
	@${DOCUMENT} src TietzeTrekker.h ${OUT}

${DOC}/TietzeTrekker.h.dvi: ${IN}/TietzeTrekker.h.pamphlet ${DOC}/axiom.sty
	@echo 72 making ${DOC}/TietzeTrekker.h.dvi from ${IN}/TietzeTrekker.h.pamphlet 
	@${DOCUMENT} dvi TietzeTrekker.h ${DOC} TietzeTrekker.h ${NOISE}


${OUT}/TTP.h: ${IN}/TTP.h.pamphlet
	@echo 73 making ${OUT}/TTP.h from ${IN}/TTP.h.pamphlet
	@${DOCUMENT} src TTP.h ${OUT}

${DOC}/TTP.h.dvi: ${IN}/TTP.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/TTP.h.dvi from ${IN}/TTP.h.pamphlet 
	@${DOCUMENT} dvi TTP.h ${DOC} TTP.h ${NOISE}


${OUT}/WordEnumerator.h: ${IN}/WordEnumerator.h.pamphlet
	@echo 75 making ${OUT}/WordEnumerator.h from ${IN}/WordEnumerator.h.pamphlet
	@${DOCUMENT} src WordEnumerator.h ${OUT}

${DOC}/WordEnumerator.h.dvi: ${IN}/WordEnumerator.h.pamphlet ${DOC}/axiom.sty
	@echo 76 making ${DOC}/WordEnumerator.h.dvi from ${IN}/WordEnumerator.h.pamphlet 
	@${DOCUMENT} dvi WordEnumerator.h ${DOC} WordEnumerator.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 77 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
