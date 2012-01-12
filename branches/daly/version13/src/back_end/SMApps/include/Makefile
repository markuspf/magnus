IN=${SRC}/backend/SMApps/include
OUT=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/SMApps/include


FILES= ${OUT}/AbelianInvariants.h ${OUT}/AbelianProblems.h \
       ${OUT}/ACConjecture.h ${OUT}/ACE.h ${OUT}/AGModule.h \
       ${OUT}/AreEltsEqual.h \
       ${OUT}/CommutatorIterator.h ${OUT}/CommutatorsChecker.h \
       ${OUT}/ConjugacyProblem.h ${OUT}/ConjugatorIterator.h \
       ${OUT}/EquationsInFPProblem.h ${OUT}/ExtendToHomProblem.h \
       ${OUT}/fastProblems.h ${OUT}/FNWP.h \
       ${OUT}/FreeIsPartOfBasisProblem.h ${OUT}/FreeProblems.h \
       ${OUT}/GAConjProblemForORGroup.h ${OUT}/GAEquations.h \
       ${OUT}/GAWordProblemForORGroup.h ${OUT}/GeneticProblems.h \
       ${OUT}/HNNProblems.h ${OUT}/HomologyProblem.h \
       ${OUT}/HToddCoxeter.h ${OUT}/IsAbelianProblem.h \
       ${OUT}/IsEltCentral.h ${OUT}/IsFiniteProblem.h \
       ${OUT}/IsFreeProblem.h ${OUT}/IsNilpotentProblem.h \
       ${OUT}/IsTrivialProblem.h ${OUT}/IsWordAPE.h \
       ${OUT}/KBModule.h ${OUT}/KernelPresentation.h \
       ${OUT}/MSCProblems.h ${OUT}/NCProblem.h \
       ${OUT}/NGSubgroupProblems.h ${OUT}/NilpotentProblems.h \
       ${OUT}/NilpotentQuotients.h ${OUT}/NormalClosure.h \
       ${OUT}/OneRelatorProblems.h \
       ${OUT}/OrderOfElt.h \
       ${OUT}/OrderProblem.h ${OUT}/QuadEquationSolver.h \
       ${OUT}/RankOfElt.h ${OUT}/RankOfSubgroup.h \
       ${OUT}/Rewrites.h \
       ${OUT}/SetOfWordsChecker.h ${OUT}/SGNilpotentQuotients.h \
       ${OUT}/SMMagnusBreakdown.h \
       ${OUT}/SubgroupContainmentProblem.h \
       ${OUT}/SubgroupProblems.h ${OUT}/ToddCoxeter.h \
       ${OUT}/TTProblem.h \
       ${OUT}/WhiteheadMinimal.h ${OUT}/WordProblem.h

DOCFILES= ${DOC}/AbelianInvariants.h.dvi ${DOC}/AbelianProblems.h.dvi \
          ${DOC}/ACConjecture.h.dvi ${DOC}/ACE.h.dvi ${DOC}/AGModule.h.dvi \
          ${DOC}/AreEltsEqual.h.dvi \
          ${DOC}/CommutatorIterator.h.dvi ${DOC}/CommutatorsChecker.h.dvi \
          ${DOC}/ConjugacyProblem.h.dvi ${DOC}/ConjugatorIterator.h.dvi \
          ${DOC}/EquationsInFPProblem.h.dvi ${DOC}/ExtendToHomProblem.h.dvi \
          ${DOC}/fastProblems.h.dvi ${DOC}/FNWP.h.dvi \
          ${DOC}/FreeIsPartOfBasisProblem.h.dvi ${DOC}/FreeProblems.h.dvi \
          ${DOC}/GAConjProblemForORGroup.h.dvi ${DOC}/GAEquations.h.dvi \
          ${DOC}/GAWordProblemForORGroup.h.dvi ${DOC}/GeneticProblems.h.dvi \
          ${DOC}/HNNProblems.h.dvi ${DOC}/HomologyProblem.h.dvi \
          ${DOC}/HToddCoxeter.h.dvi ${DOC}/IsAbelianProblem.h.dvi \
          ${DOC}/IsEltCentral.h.dvi ${DOC}/IsFiniteProblem.h.dvi \
          ${DOC}/IsFreeProblem.h.dvi ${DOC}/IsNilpotentProblem.h.dvi \
          ${DOC}/IsTrivialProblem.h.dvi ${DOC}/IsWordAPE.h.dvi \
          ${DOC}/KBModule.h.dvi ${DOC}/KernelPresentation.h.dvi \
          ${DOC}/MSCProblems.h.dvi ${DOC}/NCProblem.h.dvi \
          ${DOC}/NGSubgroupProblems.h.dvi ${DOC}/NilpotentProblems.h.dvi \
          ${DOC}/NilpotentQuotients.h.dvi ${DOC}/NormalClosure.h.dvi \
          ${DOC}/OneRelatorProblems.h.dvi \
          ${DOC}/OrderOfElt.h.dvi \
          ${DOC}/OrderProblem.h.dvi ${DOC}/QuadEquationSolver.h.dvi \
          ${DOC}/RankOfElt.h.dvi ${DOC}/RankOfSubgroup.h.dvi \
          ${DOC}/Rewrites.h.dvi \
          ${DOC}/SetOfWordsChecker.h.dvi ${DOC}/SGNilpotentQuotients.h.dvi \
          ${DOC}/SMMagnusBreakdown.h.dvi \
          ${DOC}/SubgroupContainmentProblem.h.dvi \
          ${DOC}/SubgroupProblems.h.dvi ${DOC}/ToddCoxeter.h.dvi \
          ${DOC}/TTProblem.h.dvi \
          ${DOC}/WhiteheadMinimal.h.dvi ${DOC}/WordProblem.h.dvi

all: ${FILES} ${DOCFILES}
	@echo 120 finished ${IN}

${OUT}/AbelianInvariants.h: ${IN}/AbelianInvariants.h.pamphlet
	@echo 1 making ${OUT}/AbelianInvariants.h from ${IN}/AbelianInvariants.h.pamphlet
	@${DOCUMENT} src AbelianInvariants.h ${OUT}

${DOC}/AbelianInvariants.h.dvi: ${IN}/AbelianInvariants.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/AbelianInvariants.h.dvi from ${IN}/AbelianInvariants.h.pamphlet 
	@${DOCUMENT} dvi AbelianInvariants.h ${DOC} AbelianInvariants.h ${NOISE}


${OUT}/AbelianProblems.h: ${IN}/AbelianProblems.h.pamphlet
	@echo 3 making ${OUT}/AbelianProblems.h from ${IN}/AbelianProblems.h.pamphlet
	@${DOCUMENT} src AbelianProblems.h ${OUT}

${DOC}/AbelianProblems.h.dvi: ${IN}/AbelianProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/AbelianProblems.h.dvi from ${IN}/AbelianProblems.h.pamphlet 
	@${DOCUMENT} dvi AbelianProblems.h ${DOC} AbelianProblems.h ${NOISE}


${OUT}/ACConjecture.h: ${IN}/ACConjecture.h.pamphlet
	@echo 5 making ${OUT}/ACConjecture.h from ${IN}/ACConjecture.h.pamphlet
	@${DOCUMENT} src ACConjecture.h ${OUT}

${DOC}/ACConjecture.h.dvi: ${IN}/ACConjecture.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/ACConjecture.h.dvi from ${IN}/ACConjecture.h.pamphlet 
	@${DOCUMENT} dvi ACConjecture.h ${DOC} ACConjecture.h ${NOISE}


${OUT}/ACE.h: ${IN}/ACE.h.pamphlet
	@echo 7 making ${OUT}/ACE.h from ${IN}/ACE.h.pamphlet
	@${DOCUMENT} src ACE.h ${OUT}

${DOC}/ACE.h.dvi: ${IN}/ACE.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/ACE.h.dvi from ${IN}/ACE.h.pamphlet 
	@${DOCUMENT} dvi ACE.h ${DOC} ACE.h ${NOISE}


${OUT}/AGModule.h: ${IN}/AGModule.h.pamphlet
	@echo 9 making ${OUT}/AGModule.h from ${IN}/AGModule.h.pamphlet
	@${DOCUMENT} src AGModule.h ${OUT}

${DOC}/AGModule.h.dvi: ${IN}/AGModule.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/AGModule.h.dvi from ${IN}/AGModule.h.pamphlet 
	@${DOCUMENT} dvi AGModule.h ${DOC} AGModule.h ${NOISE}


${OUT}/AreEltsEqual.h: ${IN}/AreEltsEqual.h.pamphlet
	@echo 11 making ${OUT}/AreEltsEqual.h from ${IN}/AreEltsEqual.h.pamphlet
	@${DOCUMENT} src AreEltsEqual.h ${OUT}

${DOC}/AreEltsEqual.h.dvi: ${IN}/AreEltsEqual.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/AreEltsEqual.h.dvi from ${IN}/AreEltsEqual.h.pamphlet 
	@${DOCUMENT} dvi AreEltsEqual.h ${DOC} AreEltsEqual.h ${NOISE}


${OUT}/CommutatorIterator.h: ${IN}/CommutatorIterator.h.pamphlet
	@echo 15 making ${OUT}/CommutatorIterator.h from ${IN}/CommutatorIterator.h.pamphlet
	@${DOCUMENT} src CommutatorIterator.h ${OUT}

${DOC}/CommutatorIterator.h.dvi: ${IN}/CommutatorIterator.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/CommutatorIterator.h.dvi from ${IN}/CommutatorIterator.h.pamphlet 
	@${DOCUMENT} dvi CommutatorIterator.h ${DOC} CommutatorIterator.h ${NOISE}


${OUT}/CommutatorsChecker.h: ${IN}/CommutatorsChecker.h.pamphlet
	@echo 17 making ${OUT}/CommutatorsChecker.h from ${IN}/CommutatorsChecker.h.pamphlet
	@${DOCUMENT} src CommutatorsChecker.h ${OUT}

${DOC}/CommutatorsChecker.h.dvi: ${IN}/CommutatorsChecker.h.pamphlet ${DOC}/axiom.sty
	@echo 18 making ${DOC}/CommutatorsChecker.h.dvi from ${IN}/CommutatorsChecker.h.pamphlet 
	@${DOCUMENT} dvi CommutatorsChecker.h ${DOC} CommutatorsChecker.h ${NOISE}


${OUT}/ConjugacyProblem.h: ${IN}/ConjugacyProblem.h.pamphlet
	@echo 19 making ${OUT}/ConjugacyProblem.h from ${IN}/ConjugacyProblem.h.pamphlet
	@${DOCUMENT} src ConjugacyProblem.h ${OUT}

${DOC}/ConjugacyProblem.h.dvi: ${IN}/ConjugacyProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 20 making ${DOC}/ConjugacyProblem.h.dvi from ${IN}/ConjugacyProblem.h.pamphlet 
	@${DOCUMENT} dvi ConjugacyProblem.h ${DOC} ConjugacyProblem.h ${NOISE}


${OUT}/ConjugatorIterator.h: ${IN}/ConjugatorIterator.h.pamphlet
	@echo 21 making ${OUT}/ConjugatorIterator.h from ${IN}/ConjugatorIterator.h.pamphlet
	@${DOCUMENT} src ConjugatorIterator.h ${OUT}

${DOC}/ConjugatorIterator.h.dvi: ${IN}/ConjugatorIterator.h.pamphlet ${DOC}/axiom.sty
	@echo 22 making ${DOC}/ConjugatorIterator.h.dvi from ${IN}/ConjugatorIterator.h.pamphlet 
	@${DOCUMENT} dvi ConjugatorIterator.h ${DOC} ConjugatorIterator.h ${NOISE}


${OUT}/EquationsInFPProblem.h: ${IN}/EquationsInFPProblem.h.pamphlet
	@echo 23 making ${OUT}/EquationsInFPProblem.h from ${IN}/EquationsInFPProblem.h.pamphlet
	@${DOCUMENT} src EquationsInFPProblem.h ${OUT}

${DOC}/EquationsInFPProblem.h.dvi: ${IN}/EquationsInFPProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 24 making ${DOC}/EquationsInFPProblem.h.dvi from ${IN}/EquationsInFPProblem.h.pamphlet 
	@${DOCUMENT} dvi EquationsInFPProblem.h ${DOC} EquationsInFPProblem.h ${NOISE}


${OUT}/ExtendToHomProblem.h: ${IN}/ExtendToHomProblem.h.pamphlet
	@echo 25 making ${OUT}/ExtendToHomProblem.h from ${IN}/ExtendToHomProblem.h.pamphlet
	@${DOCUMENT} src ExtendToHomProblem.h ${OUT}

${DOC}/ExtendToHomProblem.h.dvi: ${IN}/ExtendToHomProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 26 making ${DOC}/ExtendToHomProblem.h.dvi from ${IN}/ExtendToHomProblem.h.pamphlet 
	@${DOCUMENT} dvi ExtendToHomProblem.h ${DOC} ExtendToHomProblem.h ${NOISE}


${OUT}/fastProblems.h: ${IN}/fastProblems.h.pamphlet
	@echo 27 making ${OUT}/fastProblems.h from ${IN}/fastProblems.h.pamphlet
	@${DOCUMENT} src fastProblems.h ${OUT}

${DOC}/fastProblems.h.dvi: ${IN}/fastProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 28 making ${DOC}/fastProblems.h.dvi from ${IN}/fastProblems.h.pamphlet 
	@${DOCUMENT} dvi fastProblems.h ${DOC} fastProblems.h ${NOISE}


${OUT}/FNWP.h: ${IN}/FNWP.h.pamphlet
	@echo 29 making ${OUT}/FNWP.h from ${IN}/FNWP.h.pamphlet
	@${DOCUMENT} src FNWP.h ${OUT}

${DOC}/FNWP.h.dvi: ${IN}/FNWP.h.pamphlet ${DOC}/axiom.sty
	@echo 30 making ${DOC}/FNWP.h.dvi from ${IN}/FNWP.h.pamphlet 
	@${DOCUMENT} dvi FNWP.h ${DOC} FNWP.h ${NOISE}


${OUT}/FreeIsPartOfBasisProblem.h: ${IN}/FreeIsPartOfBasisProblem.h.pamphlet
	@echo 31 making ${OUT}/FreeIsPartOfBasisProblem.h from ${IN}/FreeIsPartOfBasisProblem.h.pamphlet
	@${DOCUMENT} src FreeIsPartOfBasisProblem.h ${OUT}

${DOC}/FreeIsPartOfBasisProblem.h.dvi: ${IN}/FreeIsPartOfBasisProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 32 making ${DOC}/FreeIsPartOfBasisProblem.h.dvi from ${IN}/FreeIsPartOfBasisProblem.h.pamphlet 
	@${DOCUMENT} dvi FreeIsPartOfBasisProblem.h ${DOC} FreeIsPartOfBasisProblem.h ${NOISE}


${OUT}/FreeProblems.h: ${IN}/FreeProblems.h.pamphlet
	@echo 33 making ${OUT}/FreeProblems.h from ${IN}/FreeProblems.h.pamphlet
	@${DOCUMENT} src FreeProblems.h ${OUT}

${DOC}/FreeProblems.h.dvi: ${IN}/FreeProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 34 making ${DOC}/FreeProblems.h.dvi from ${IN}/FreeProblems.h.pamphlet 
	@${DOCUMENT} dvi FreeProblems.h ${DOC} FreeProblems.h ${NOISE}


${OUT}/GAConjProblemForORGroup.h: ${IN}/GAConjProblemForORGroup.h.pamphlet
	@echo 35 making ${OUT}/GAConjProblemForORGroup.h from ${IN}/GAConjProblemForORGroup.h.pamphlet
	@${DOCUMENT} src GAConjProblemForORGroup.h ${OUT}

${DOC}/GAConjProblemForORGroup.h.dvi: ${IN}/GAConjProblemForORGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 36 making ${DOC}/GAConjProblemForORGroup.h.dvi from ${IN}/GAConjProblemForORGroup.h.pamphlet 
	@${DOCUMENT} dvi GAConjProblemForORGroup.h ${DOC} GAConjProblemForORGroup.h ${NOISE}


${OUT}/GAEquations.h: ${IN}/GAEquations.h.pamphlet
	@echo 37 making ${OUT}/GAEquations.h from ${IN}/GAEquations.h.pamphlet
	@${DOCUMENT} src GAEquations.h ${OUT}

${DOC}/GAEquations.h.dvi: ${IN}/GAEquations.h.pamphlet ${DOC}/axiom.sty
	@echo 38 making ${DOC}/GAEquations.h.dvi from ${IN}/GAEquations.h.pamphlet 
	@${DOCUMENT} dvi GAEquations.h ${DOC} GAEquations.h ${NOISE}


${OUT}/GAWordProblemForORGroup.h: ${IN}/GAWordProblemForORGroup.h.pamphlet
	@echo 39 making ${OUT}/GAWordProblemForORGroup.h from ${IN}/GAWordProblemForORGroup.h.pamphlet
	@${DOCUMENT} src GAWordProblemForORGroup.h ${OUT}

${DOC}/GAWordProblemForORGroup.h.dvi: ${IN}/GAWordProblemForORGroup.h.pamphlet ${DOC}/axiom.sty
	@echo 40 making ${DOC}/GAWordProblemForORGroup.h.dvi from ${IN}/GAWordProblemForORGroup.h.pamphlet 
	@${DOCUMENT} dvi GAWordProblemForORGroup.h ${DOC} GAWordProblemForORGroup.h ${NOISE}


${OUT}/GeneticProblems.h: ${IN}/GeneticProblems.h.pamphlet
	@echo 41 making ${OUT}/GeneticProblems.h from ${IN}/GeneticProblems.h.pamphlet
	@${DOCUMENT} src GeneticProblems.h ${OUT}

${DOC}/GeneticProblems.h.dvi: ${IN}/GeneticProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 42 making ${DOC}/GeneticProblems.h.dvi from ${IN}/GeneticProblems.h.pamphlet 
	@${DOCUMENT} dvi GeneticProblems.h ${DOC} GeneticProblems.h ${NOISE}


${OUT}/HNNProblems.h: ${IN}/HNNProblems.h.pamphlet
	@echo 43 making ${OUT}/HNNProblems.h from ${IN}/HNNProblems.h.pamphlet
	@${DOCUMENT} src HNNProblems.h ${OUT}

${DOC}/HNNProblems.h.dvi: ${IN}/HNNProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 44 making ${DOC}/HNNProblems.h.dvi from ${IN}/HNNProblems.h.pamphlet 
	@${DOCUMENT} dvi HNNProblems.h ${DOC} HNNProblems.h ${NOISE}


${OUT}/HomologyProblem.h: ${IN}/HomologyProblem.h.pamphlet
	@echo 45 making ${OUT}/HomologyProblem.h from ${IN}/HomologyProblem.h.pamphlet
	@${DOCUMENT} src HomologyProblem.h ${OUT}

${DOC}/HomologyProblem.h.dvi: ${IN}/HomologyProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 46 making ${DOC}/HomologyProblem.h.dvi from ${IN}/HomologyProblem.h.pamphlet 
	@${DOCUMENT} dvi HomologyProblem.h ${DOC} HomologyProblem.h ${NOISE}


${OUT}/HToddCoxeter.h: ${IN}/HToddCoxeter.h.pamphlet
	@echo 47 making ${OUT}/HToddCoxeter.h from ${IN}/HToddCoxeter.h.pamphlet
	@${DOCUMENT} src HToddCoxeter.h ${OUT}

${DOC}/HToddCoxeter.h.dvi: ${IN}/HToddCoxeter.h.pamphlet ${DOC}/axiom.sty
	@echo 48 making ${DOC}/HToddCoxeter.h.dvi from ${IN}/HToddCoxeter.h.pamphlet 
	@${DOCUMENT} dvi HToddCoxeter.h ${DOC} HToddCoxeter.h ${NOISE}


${OUT}/IsAbelianProblem.h: ${IN}/IsAbelianProblem.h.pamphlet
	@echo 49 making ${OUT}/IsAbelianProblem.h from ${IN}/IsAbelianProblem.h.pamphlet
	@${DOCUMENT} src IsAbelianProblem.h ${OUT}

${DOC}/IsAbelianProblem.h.dvi: ${IN}/IsAbelianProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 50 making ${DOC}/IsAbelianProblem.h.dvi from ${IN}/IsAbelianProblem.h.pamphlet 
	@${DOCUMENT} dvi IsAbelianProblem.h ${DOC} IsAbelianProblem.h ${NOISE}


${OUT}/IsEltCentral.h: ${IN}/IsEltCentral.h.pamphlet
	@echo 51 making ${OUT}/IsEltCentral.h from ${IN}/IsEltCentral.h.pamphlet
	@${DOCUMENT} src IsEltCentral.h ${OUT}

${DOC}/IsEltCentral.h.dvi: ${IN}/IsEltCentral.h.pamphlet ${DOC}/axiom.sty
	@echo 52 making ${DOC}/IsEltCentral.h.dvi from ${IN}/IsEltCentral.h.pamphlet 
	@${DOCUMENT} dvi IsEltCentral.h ${DOC} IsEltCentral.h ${NOISE}


${OUT}/IsFiniteProblem.h: ${IN}/IsFiniteProblem.h.pamphlet
	@echo 53 making ${OUT}/IsFiniteProblem.h from ${IN}/IsFiniteProblem.h.pamphlet
	@${DOCUMENT} src IsFiniteProblem.h ${OUT}

${DOC}/IsFiniteProblem.h.dvi: ${IN}/IsFiniteProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 54 making ${DOC}/IsFiniteProblem.h.dvi from ${IN}/IsFiniteProblem.h.pamphlet 
	@${DOCUMENT} dvi IsFiniteProblem.h ${DOC} IsFiniteProblem.h ${NOISE}


${OUT}/IsFreeProblem.h: ${IN}/IsFreeProblem.h.pamphlet
	@echo 55 making ${OUT}/IsFreeProblem.h from ${IN}/IsFreeProblem.h.pamphlet
	@${DOCUMENT} src IsFreeProblem.h ${OUT}

${DOC}/IsFreeProblem.h.dvi: ${IN}/IsFreeProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 56 making ${DOC}/IsFreeProblem.h.dvi from ${IN}/IsFreeProblem.h.pamphlet 
	@${DOCUMENT} dvi IsFreeProblem.h ${DOC} IsFreeProblem.h ${NOISE}


${OUT}/IsNilpotentProblem.h: ${IN}/IsNilpotentProblem.h.pamphlet
	@echo 57 making ${OUT}/IsNilpotentProblem.h from ${IN}/IsNilpotentProblem.h.pamphlet
	@${DOCUMENT} src IsNilpotentProblem.h ${OUT}

${DOC}/IsNilpotentProblem.h.dvi: ${IN}/IsNilpotentProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 58 making ${DOC}/IsNilpotentProblem.h.dvi from ${IN}/IsNilpotentProblem.h.pamphlet 
	@${DOCUMENT} dvi IsNilpotentProblem.h ${DOC} IsNilpotentProblem.h ${NOISE}


${OUT}/IsTrivialProblem.h: ${IN}/IsTrivialProblem.h.pamphlet
	@echo 59 making ${OUT}/IsTrivialProblem.h from ${IN}/IsTrivialProblem.h.pamphlet
	@${DOCUMENT} src IsTrivialProblem.h ${OUT}

${DOC}/IsTrivialProblem.h.dvi: ${IN}/IsTrivialProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 60 making ${DOC}/IsTrivialProblem.h.dvi from ${IN}/IsTrivialProblem.h.pamphlet 
	@${DOCUMENT} dvi IsTrivialProblem.h ${DOC} IsTrivialProblem.h ${NOISE}


${OUT}/IsWordAPE.h: ${IN}/IsWordAPE.h.pamphlet
	@echo 61 making ${OUT}/IsWordAPE.h from ${IN}/IsWordAPE.h.pamphlet
	@${DOCUMENT} src IsWordAPE.h ${OUT}

${DOC}/IsWordAPE.h.dvi: ${IN}/IsWordAPE.h.pamphlet ${DOC}/axiom.sty
	@echo 62 making ${DOC}/IsWordAPE.h.dvi from ${IN}/IsWordAPE.h.pamphlet 
	@${DOCUMENT} dvi IsWordAPE.h ${DOC} IsWordAPE.h ${NOISE}


${OUT}/KBModule.h: ${IN}/KBModule.h.pamphlet
	@echo 73 making ${OUT}/KBModule.h from ${IN}/KBModule.h.pamphlet
	@${DOCUMENT} src KBModule.h ${OUT}

${DOC}/KBModule.h.dvi: ${IN}/KBModule.h.pamphlet ${DOC}/axiom.sty
	@echo 64 making ${DOC}/KBModule.h.dvi from ${IN}/KBModule.h.pamphlet 
	@${DOCUMENT} dvi KBModule.h ${DOC} KBModule.h ${NOISE}


${OUT}/KernelPresentation.h: ${IN}/KernelPresentation.h.pamphlet
	@echo 65 making ${OUT}/KernelPresentation.h from ${IN}/KernelPresentation.h.pamphlet
	@${DOCUMENT} src KernelPresentation.h ${OUT}

${DOC}/KernelPresentation.h.dvi: ${IN}/KernelPresentation.h.pamphlet ${DOC}/axiom.sty
	@echo 66 making ${DOC}/KernelPresentation.h.dvi from ${IN}/KernelPresentation.h.pamphlet 
	@${DOCUMENT} dvi KernelPresentation.h ${DOC} KernelPresentation.h ${NOISE}


${OUT}/MSCProblems.h: ${IN}/MSCProblems.h.pamphlet
	@echo 67 making ${OUT}/MSCProblems.h from ${IN}/MSCProblems.h.pamphlet
	@${DOCUMENT} src MSCProblems.h ${OUT}

${DOC}/MSCProblems.h.dvi: ${IN}/MSCProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 68 making ${DOC}/MSCProblems.h.dvi from ${IN}/MSCProblems.h.pamphlet 
	@${DOCUMENT} dvi MSCProblems.h ${DOC} MSCProblems.h ${NOISE}


${OUT}/NCProblem.h: ${IN}/NCProblem.h.pamphlet
	@echo 69 making ${OUT}/NCProblem.h from ${IN}/NCProblem.h.pamphlet
	@${DOCUMENT} src NCProblem.h ${OUT}

${DOC}/NCProblem.h.dvi: ${IN}/NCProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 70 making ${DOC}/NCProblem.h.dvi from ${IN}/NCProblem.h.pamphlet 
	@${DOCUMENT} dvi NCProblem.h ${DOC} NCProblem.h ${NOISE}


${OUT}/NGSubgroupProblems.h: ${IN}/NGSubgroupProblems.h.pamphlet
	@echo 71 making ${OUT}/NGSubgroupProblems.h from ${IN}/NGSubgroupProblems.h.pamphlet
	@${DOCUMENT} src NGSubgroupProblems.h ${OUT}

${DOC}/NGSubgroupProblems.h.dvi: ${IN}/NGSubgroupProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 72 making ${DOC}/NGSubgroupProblems.h.dvi from ${IN}/NGSubgroupProblems.h.pamphlet 
	@${DOCUMENT} dvi NGSubgroupProblems.h ${DOC} NGSubgroupProblems.h ${NOISE}


${OUT}/NilpotentProblems.h: ${IN}/NilpotentProblems.h.pamphlet
	@echo 73 making ${OUT}/NilpotentProblems.h from ${IN}/NilpotentProblems.h.pamphlet
	@${DOCUMENT} src NilpotentProblems.h ${OUT}

${DOC}/NilpotentProblems.h.dvi: ${IN}/NilpotentProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 74 making ${DOC}/NilpotentProblems.h.dvi from ${IN}/NilpotentProblems.h.pamphlet 
	@${DOCUMENT} dvi NilpotentProblems.h ${DOC} NilpotentProblems.h ${NOISE}


${OUT}/NilpotentQuotients.h: ${IN}/NilpotentQuotients.h.pamphlet
	@echo 75 making ${OUT}/NilpotentQuotients.h from ${IN}/NilpotentQuotients.h.pamphlet
	@${DOCUMENT} src NilpotentQuotients.h ${OUT}

${DOC}/NilpotentQuotients.h.dvi: ${IN}/NilpotentQuotients.h.pamphlet ${DOC}/axiom.sty
	@echo 76 making ${DOC}/NilpotentQuotients.h.dvi from ${IN}/NilpotentQuotients.h.pamphlet 
	@${DOCUMENT} dvi NilpotentQuotients.h ${DOC} NilpotentQuotients.h ${NOISE}


${OUT}/NormalClosure.h: ${IN}/NormalClosure.h.pamphlet
	@echo 77 making ${OUT}/NormalClosure.h from ${IN}/NormalClosure.h.pamphlet
	@${DOCUMENT} src NormalClosure.h ${OUT}

${DOC}/NormalClosure.h.dvi: ${IN}/NormalClosure.h.pamphlet ${DOC}/axiom.sty
	@echo 78 making ${DOC}/NormalClosure.h.dvi from ${IN}/NormalClosure.h.pamphlet 
	@${DOCUMENT} dvi NormalClosure.h ${DOC} NormalClosure.h ${NOISE}


${OUT}/OneRelatorProblems.h: ${IN}/OneRelatorProblems.h.pamphlet
	@echo 79 making ${OUT}/OneRelatorProblems.h from ${IN}/OneRelatorProblems.h.pamphlet
	@${DOCUMENT} src OneRelatorProblems.h ${OUT}

${DOC}/OneRelatorProblems.h.dvi: ${IN}/OneRelatorProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 80 making ${DOC}/OneRelatorProblems.h.dvi from ${IN}/OneRelatorProblems.h.pamphlet 
	@${DOCUMENT} dvi OneRelatorProblems.h ${DOC} OneRelatorProblems.h ${NOISE}


${OUT}/OrderOfElt.h: ${IN}/OrderOfElt.h.pamphlet
	@echo 83 making ${OUT}/OrderOfElt.h from ${IN}/OrderOfElt.h.pamphlet
	@${DOCUMENT} src OrderOfElt.h ${OUT}

${DOC}/OrderOfElt.h.dvi: ${IN}/OrderOfElt.h.pamphlet ${DOC}/axiom.sty
	@echo 84 making ${DOC}/OrderOfElt.h.dvi from ${IN}/OrderOfElt.h.pamphlet 
	@${DOCUMENT} dvi OrderOfElt.h ${DOC} OrderOfElt.h ${NOISE}


${OUT}/OrderProblem.h: ${IN}/OrderProblem.h.pamphlet
	@echo 85 making ${OUT}/OrderProblem.h from ${IN}/OrderProblem.h.pamphlet
	@${DOCUMENT} src OrderProblem.h ${OUT}

${DOC}/OrderProblem.h.dvi: ${IN}/OrderProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 86 making ${DOC}/OrderProblem.h.dvi from ${IN}/OrderProblem.h.pamphlet 
	@${DOCUMENT} dvi OrderProblem.h ${DOC} OrderProblem.h ${NOISE}


${OUT}/QuadEquationSolver.h: ${IN}/QuadEquationSolver.h.pamphlet
	@echo 87 making ${OUT}/QuadEquationSolver.h from ${IN}/QuadEquationSolver.h.pamphlet
	@${DOCUMENT} src QuadEquationSolver.h ${OUT}

${DOC}/QuadEquationSolver.h.dvi: ${IN}/QuadEquationSolver.h.pamphlet ${DOC}/axiom.sty
	@echo 88 making ${DOC}/QuadEquationSolver.h.dvi from ${IN}/QuadEquationSolver.h.pamphlet 
	@${DOCUMENT} dvi QuadEquationSolver.h ${DOC} QuadEquationSolver.h ${NOISE}


${OUT}/RankOfElt.h: ${IN}/RankOfElt.h.pamphlet
	@echo 89 making ${OUT}/RankOfElt.h from ${IN}/RankOfElt.h.pamphlet
	@${DOCUMENT} src RankOfElt.h ${OUT}

${DOC}/RankOfElt.h.dvi: ${IN}/RankOfElt.h.pamphlet ${DOC}/axiom.sty
	@echo 90 making ${DOC}/RankOfElt.h.dvi from ${IN}/RankOfElt.h.pamphlet 
	@${DOCUMENT} dvi RankOfElt.h ${DOC} RankOfElt.h ${NOISE}


${OUT}/RankOfSubgroup.h: ${IN}/RankOfSubgroup.h.pamphlet
	@echo 91 making ${OUT}/RankOfSubgroup.h from ${IN}/RankOfSubgroup.h.pamphlet
	@${DOCUMENT} src RankOfSubgroup.h ${OUT}

${DOC}/RankOfSubgroup.h.dvi: ${IN}/RankOfSubgroup.h.pamphlet ${DOC}/axiom.sty
	@echo 92 making ${DOC}/RankOfSubgroup.h.dvi from ${IN}/RankOfSubgroup.h.pamphlet 
	@${DOCUMENT} dvi RankOfSubgroup.h ${DOC} RankOfSubgroup.h ${NOISE}


${OUT}/Rewrites.h: ${IN}/Rewrites.h.pamphlet
	@echo 95 making ${OUT}/Rewrites.h from ${IN}/Rewrites.h.pamphlet
	@${DOCUMENT} src Rewrites.h ${OUT}

${DOC}/Rewrites.h.dvi: ${IN}/Rewrites.h.pamphlet ${DOC}/axiom.sty
	@echo 96 making ${DOC}/Rewrites.h.dvi from ${IN}/Rewrites.h.pamphlet 
	@${DOCUMENT} dvi Rewrites.h ${DOC} Rewrites.h ${NOISE}


${OUT}/SetOfWordsChecker.h: ${IN}/SetOfWordsChecker.h.pamphlet
	@echo 97 making ${OUT}/SetOfWordsChecker.h from ${IN}/SetOfWordsChecker.h.pamphlet
	@${DOCUMENT} src SetOfWordsChecker.h ${OUT}

${DOC}/SetOfWordsChecker.h.dvi: ${IN}/SetOfWordsChecker.h.pamphlet ${DOC}/axiom.sty
	@echo 98 making ${DOC}/SetOfWordsChecker.h.dvi from ${IN}/SetOfWordsChecker.h.pamphlet 
	@${DOCUMENT} dvi SetOfWordsChecker.h ${DOC} SetOfWordsChecker.h ${NOISE}


${OUT}/SGNilpotentQuotients.h: ${IN}/SGNilpotentQuotients.h.pamphlet
	@echo 99 making ${OUT}/SGNilpotentQuotients.h from ${IN}/SGNilpotentQuotients.h.pamphlet
	@${DOCUMENT} src SGNilpotentQuotients.h ${OUT}

${DOC}/SGNilpotentQuotients.h.dvi: ${IN}/SGNilpotentQuotients.h.pamphlet ${DOC}/axiom.sty
	@echo 100 making ${DOC}/SGNilpotentQuotients.h.dvi from ${IN}/SGNilpotentQuotients.h.pamphlet 
	@${DOCUMENT} dvi SGNilpotentQuotients.h ${DOC} SGNilpotentQuotients.h ${NOISE}


${OUT}/SMMagnusBreakdown.h: ${IN}/SMMagnusBreakdown.h.pamphlet
	@echo 103 making ${OUT}/SMMagnusBreakdown.h from ${IN}/SMMagnusBreakdown.h.pamphlet
	@${DOCUMENT} src SMMagnusBreakdown.h ${OUT}

${DOC}/SMMagnusBreakdown.h.dvi: ${IN}/SMMagnusBreakdown.h.pamphlet ${DOC}/axiom.sty
	@echo 104 making ${DOC}/SMMagnusBreakdown.h.dvi from ${IN}/SMMagnusBreakdown.h.pamphlet 
	@${DOCUMENT} dvi SMMagnusBreakdown.h ${DOC} SMMagnusBreakdown.h ${NOISE}


${OUT}/SubgroupContainmentProblem.h: ${IN}/SubgroupContainmentProblem.h.pamphlet
	@echo 105 making ${OUT}/SubgroupContainmentProblem.h from ${IN}/SubgroupContainmentProblem.h.pamphlet
	@${DOCUMENT} src SubgroupContainmentProblem.h ${OUT}

${DOC}/SubgroupContainmentProblem.h.dvi: ${IN}/SubgroupContainmentProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 106 making ${DOC}/SubgroupContainmentProblem.h.dvi from ${IN}/SubgroupContainmentProblem.h.pamphlet 
	@${DOCUMENT} dvi SubgroupContainmentProblem.h ${DOC} SubgroupContainmentProblem.h ${NOISE}


${OUT}/SubgroupProblems.h: ${IN}/SubgroupProblems.h.pamphlet
	@echo 107 making ${OUT}/SubgroupProblems.h from ${IN}/SubgroupProblems.h.pamphlet
	@${DOCUMENT} src SubgroupProblems.h ${OUT}

${DOC}/SubgroupProblems.h.dvi: ${IN}/SubgroupProblems.h.pamphlet ${DOC}/axiom.sty
	@echo 108 making ${DOC}/SubgroupProblems.h.dvi from ${IN}/SubgroupProblems.h.pamphlet 
	@${DOCUMENT} dvi SubgroupProblems.h ${DOC} SubgroupProblems.h ${NOISE}


${OUT}/ToddCoxeter.h: ${IN}/ToddCoxeter.h.pamphlet
	@echo 109 making ${OUT}/ToddCoxeter.h from ${IN}/ToddCoxeter.h.pamphlet
	@${DOCUMENT} src ToddCoxeter.h ${OUT}

${DOC}/ToddCoxeter.h.dvi: ${IN}/ToddCoxeter.h.pamphlet ${DOC}/axiom.sty
	@echo 110 making ${DOC}/ToddCoxeter.h.dvi from ${IN}/ToddCoxeter.h.pamphlet 
	@${DOCUMENT} dvi ToddCoxeter.h ${DOC} ToddCoxeter.h ${NOISE}


${OUT}/TTProblem.h: ${IN}/TTProblem.h.pamphlet
	@echo 111 making ${OUT}/TTProblem.h from ${IN}/TTProblem.h.pamphlet
	@${DOCUMENT} src TTProblem.h ${OUT}

${DOC}/TTProblem.h.dvi: ${IN}/TTProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 112 making ${DOC}/TTProblem.h.dvi from ${IN}/TTProblem.h.pamphlet 
	@${DOCUMENT} dvi TTProblem.h ${DOC} TTProblem.h ${NOISE}


${OUT}/WhiteheadMinimal.h: ${IN}/WhiteheadMinimal.h.pamphlet
	@echo 115 making ${OUT}/WhiteheadMinimal.h from ${IN}/WhiteheadMinimal.h.pamphlet
	@${DOCUMENT} src WhiteheadMinimal.h ${OUT}

${DOC}/WhiteheadMinimal.h.dvi: ${IN}/WhiteheadMinimal.h.pamphlet ${DOC}/axiom.sty
	@echo 116 making ${DOC}/WhiteheadMinimal.h.dvi from ${IN}/WhiteheadMinimal.h.pamphlet 
	@${DOCUMENT} dvi WhiteheadMinimal.h ${DOC} WhiteheadMinimal.h ${NOISE}


${OUT}/WordProblem.h: ${IN}/WordProblem.h.pamphlet
	@echo 117 making ${OUT}/WordProblem.h from ${IN}/WordProblem.h.pamphlet
	@${DOCUMENT} src WordProblem.h ${OUT}

${DOC}/WordProblem.h.dvi: ${IN}/WordProblem.h.pamphlet ${DOC}/axiom.sty
	@echo 118 making ${DOC}/WordProblem.h.dvi from ${IN}/WordProblem.h.pamphlet 
	@${DOCUMENT} dvi WordProblem.h ${DOC} WordProblem.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 119 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
