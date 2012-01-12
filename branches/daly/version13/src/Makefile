IN=${SRC}
MID=${INT}
OUT=${OBJ}/${SYS}
DOC=${MNT}/${SYS}/doc/src
BIN=${MNT}/${SYS}/bin

ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${MID} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE} 

DIRS=includedir backenddir

INCFILES=${MID}/magnus.h ${MID}/SessionManager.h ${MID}/TheObjects.h \
         ${MID}/ObjectSmith.h ${MID}/DatabaseManager.h ${MID}/FEData.h \
         ${MID}/ComputationManager.h ${MID}/OutMessages.h \
         ${MID}/GACPforORGSolver.h ${MID}/GAIsPartOfBasis.h \
         ${MID}/ACConfig.h ${MID}/GAEquationSolver.h ${MID}/Config.h \
         ${MID}/ACGA.h ${MID}/PresentationProblems.h \
         ${MID}/MakeRandomPresentation.h ${MID}/SGREnumerator.h \
         ${MID}/SMEnumerator.h ${MID}/WEnumerator.h ${MID}/REnumerator.h \
         ${MID}/AutoEnumerator.h ${MID}/SubgroupEnumerator.h \
         ${MID}/SMListIterator.h ${MID}/ORConsequencesEnumerator.h \
         ${MID}/HomEnumerators.h ${MID}/ListProblems.h \
         ${MID}/SMListWordProblem.h ${MID}/SMListSubgroupProblems.h \
         ${MID}/FreeListProblems.h ${MID}/Roulette.h \
         ${MID}/NormalRandomWord.h ${MID}/MagnusHome.h ${MID}/SMList.h \
         ${MID}/SMMap.h ${MID}/GAWP.h ${MID}/Set.h ${MID}/PackagesSMApps.h \
         ${MID}/PackagesData.h

DOCFILES=${DOC}/magnus.dvi ${DOC}/sessionmanager.dvi ${DOC}/theobjects.dvi \
         ${DOC}/objectsmith.dvi ${DOC}/databasemanager.dvi ${DOC}/fedata.dvi \
         ${DOC}/computationmanager.dvi ${DOC}/outmessages.dvi \
         ${DOC}/menudefns.dvi ${DOC}/gacpfororgsolver.dvi \
         ${DOC}/gaispartofbasis.dvi ${DOC}/acconfig.dvi \
         ${DOC}/gaequationsolver.dvi ${DOC}/config.dvi ${DOC}/acga.dvi \
         ${DOC}/presentationproblems.dvi ${DOC}/makerandompresentation.dvi \
         ${DOC}/sgrenumerator.dvi ${DOC}/smenumerator.dvi \
         ${DOC}/wenumerator.dvi ${DOC}/renumerator.dvi \
         ${DOC}/autoenumerator.dvi ${DOC}/subgroupenumerator.dvi \
         ${DOC}/smlistiterator.dvi ${DOC}/orconsequencesenumerator.dvi \
         ${DOC}/homenumerators.dvi ${DOC}/listproblems.dvi \
         ${DOC}/smlistwordproblem.dvi ${DOC}/smlistsubgroupproblems.dvi \
         ${DOC}/freelistproblems.dvi ${DOC}/normalrandomword.dvi \
         ${DOC}/magnushome.dvi ${DOC}/smlist.dvi ${DOC}/smmap.dvi \
         ${DOC}/gawp.dvi ${DOC}/registerproperties.dvi ${DOC}/set.dvi \
         ${DOC}/packagessmapps.dvi ${DOC}/packagesdata.dvi

OFILES=  ${OUT}/sessionmanager.o ${OUT}/theobjects.o ${OUT}/objectsmith.o \
         ${OUT}/databasemanager.o ${OUT}/fedata.o ${OUT}/computationmanager.o \
         ${OUT}/outmessages.o ${OUT}/menudefns.o ${OUT}/gacpfororgsolver.o \
         ${OUT}/gaispartofbasis.o ${OUT}/acconfig.o ${OUT}/gaequationsolver.o \
         ${OUT}/config.o ${OUT}/acga.o ${OUT}/presentationproblems.o \
         ${OUT}/makerandompresentation.o ${OUT}/sgrenumerator.o \
         ${OUT}/smenumerator.o ${OUT}/wenumerator.o ${OUT}/renumerator.o \
         ${OUT}/autoenumerator.o ${OUT}/subgroupenumerator.o \
         ${OUT}/smlistiterator.o ${OUT}/orconsequencesenumerator.o \
         ${OUT}/homenumerators.o ${OUT}/listproblems.o \
         ${OUT}/smlistwordproblem.o ${OUT}/smlistsubgroupproblems.o \
         ${OUT}/freelistproblems.o ${OUT}/normalrandomword.o \
         ${OUT}/magnushome.o ${OUT}/smlist.o ${OUT}/smmap.o \
         ${OUT}/gawp.o ${OUT}/registerproperties.o ${OUT}/set.o \
         ${OUT}/packagessmapps.o ${OUT}/packagesdata.o

BINFILES=${BIN}/magnus


all: ${INCFILES} ${DIRS} ${DOCFILES} ${OFILES} ${BINFILES}
	@echo 33 finished ${SRC}

includedir:
	@echo 31 making ${SRC}/include
	@ mkdir -p ${SRC}/include
	@ mkdir -p ${INT}/include
	@ mkdir -p ${OBJ}/${SYS}/test
	@ mkdir -p ${MNT}/${SYS}/doc/src/include
	@( cd ${SRC}/include ; \
	${DOCUMENT} src Makefile . Makefile ; \
	${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	${ENV} ${MAKE} )

backenddir:
	@echo 32 making ${SRC}/backend
	@( cd backend ; \
	${DOCUMENT} src Makefile . Makefile ; \
	${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	${ENV} ${MAKE} )


${MID}/ACConfig.h: ${IN}/acconfig.pamphlet
	@echo 1 making ${MID}/ACConfig.h from ${IN}/acconfig.pamphlet
	@${DOCUMENT} chunk "ACConfig.h" acconfig ${MID} ACConfig.h

${MID}/acconfig.C: ${IN}/acconfig.pamphlet
	@echo 2 making ${MID}/acconfig.C from ${IN}/acconfig.pamphlet
	@${DOCUMENT} src acconfig ${MID} acconfig.C

${OUT}/acconfig.o: ${MID}/acconfig.C
	@echo 3 making ${OUT}/acconfig.o from ${MID}/acconfig.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/acconfig.o ${MID}/acconfig.C

${DOC}/acconfig.dvi: ${IN}/acconfig.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/acconfig.dvi from ${IN}/acconfig.pamphlet 
	@${DOCUMENT} dvi acconfig ${DOC} acconfig ${NOISE}


${MID}/ACGA.h: ${IN}/acga.pamphlet
	@echo 1 making ${MID}/ACGA.h from ${IN}/acga.pamphlet
	@${DOCUMENT} chunk "ACGA.h" acga ${MID} ACGA.h

${MID}/acga.C: ${IN}/acga.pamphlet
	@echo 2 making ${MID}/acga.C from ${IN}/acga.pamphlet
	@${DOCUMENT} src acga ${MID} acga.C

${OUT}/acga.o: ${MID}/acga.C
	@echo 3 making ${OUT}/acga.o from ${MID}/acga.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/acga.o ${MID}/acga.C

${DOC}/acga.dvi: ${IN}/acga.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/acga.dvi from ${IN}/acga.pamphlet 
	@${DOCUMENT} dvi acga ${DOC} acga ${NOISE}


${MID}/ComputationManager.h: ${IN}/computationmanager.pamphlet
	@echo 1 making ${MID}/ComputationManager.h from ${IN}/computationmanager.pamphlet
	@${DOCUMENT} chunk "ComputationManager.h" computationmanager ${MID} ComputationManager.h

${MID}/computationmanager.C: ${IN}/computationmanager.pamphlet
	@echo 2 making ${MID}/computationmanager.C from ${IN}/computationmanager.pamphlet
	@${DOCUMENT} src computationmanager ${MID} computationmanager.C

${OUT}/computationmanager.o: ${MID}/computationmanager.C
	@echo 3 making ${OUT}/computationmanager.o from ${MID}/computationmanager.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/computationmanager.o ${MID}/computationmanager.C

${DOC}/computationmanager.dvi: ${IN}/computationmanager.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/computationmanager.dvi from ${IN}/computationmanager.pamphlet 
	@${DOCUMENT} dvi computationmanager ${DOC} computationmanager ${NOISE}


${MID}/AutoEnumerator.h: ${IN}/autoenumerator.pamphlet
	@echo 1 making ${MID}/AutoEnumerator.h from ${IN}/autoenumerator.pamphlet
	@${DOCUMENT} chunk "AutoEnumerator.h" autoenumerator ${MID} AutoEnumerator.h

${MID}/autoenumerator.C: ${IN}/autoenumerator.pamphlet
	@echo 2 making ${MID}/autoenumerator.C from ${IN}/autoenumerator.pamphlet
	@${DOCUMENT} src autoenumerator ${MID} autoenumerator.C

${OUT}/autoenumerator.o: ${MID}/autoenumerator.C
	@echo 3 making ${OUT}/autoenumerator.o from ${MID}/autoenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/autoenumerator.o ${MID}/autoenumerator.C

${DOC}/autoenumerator.dvi: ${IN}/autoenumerator.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/autoenumerator.dvi from ${IN}/autoenumerator.pamphlet 
	@${DOCUMENT} dvi autoenumerator ${DOC} autoenumerator ${NOISE}


${MID}/Config.h: ${IN}/config.pamphlet
	@echo 1 making ${MID}/Config.h from ${IN}/config.pamphlet
	@${DOCUMENT} chunk "Config.h" config ${MID} Config.h

${MID}/config.C: ${IN}/config.pamphlet
	@echo 2 making ${MID}/config.C from ${IN}/config.pamphlet
	@${DOCUMENT} src config ${MID} config.C

${OUT}/config.o: ${MID}/config.C
	@echo 3 making ${OUT}/config.o from ${MID}/config.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/config.o ${MID}/config.C

${DOC}/config.dvi: ${IN}/config.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/config.dvi from ${IN}/config.pamphlet 
	@${DOCUMENT} dvi config ${DOC} config ${NOISE}


${MID}/DatabaseManager.h: ${IN}/databasemanager.pamphlet
	@echo 5 making ${MID}/DatabaseManager.h from ${IN}/databasemanager.pamphlet
	@${DOCUMENT} chunk "DatabaseManager.h" databasemanager ${MID} DatabaseManager.h

${MID}/databasemanager.C: ${IN}/databasemanager.pamphlet
	@echo 6 making ${MID}/databasemanager.C from ${IN}/databasemanager.pamphlet
	@${DOCUMENT} src databasemanager ${MID} databasemanager.C

${OUT}/databasemanager.o: ${MID}/databasemanager.C
	@echo 7 making ${OUT}/databasemanager.o from ${MID}/databasemanager.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/databasemanager.o ${MID}/databasemanager.C

${DOC}/databasemanager.dvi: ${IN}/databasemanager.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/databasemanager.dvi from ${IN}/databasemanager.pamphlet 
	@${DOCUMENT} dvi databasemanager ${DOC} databasemanager ${NOISE}


${MID}/FEData.h: ${IN}/fedata.pamphlet
	@echo 9 making ${MID}/FEData.h from ${IN}/fedata.pamphlet
	@${DOCUMENT} chunk "FEData.h" fedata ${MID} FEData.h

${MID}/fedata.C: ${IN}/fedata.pamphlet
	@echo 10 making ${MID}/fedata.C from ${IN}/fedata.pamphlet
	@${DOCUMENT} src fedata ${MID} fedata.C

${OUT}/fedata.o: ${MID}/fedata.C
	@echo 11 making ${OUT}/fedata.o from ${MID}/fedata.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/fedata.o ${MID}/fedata.C

${DOC}/fedata.dvi: ${IN}/fedata.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/fedata.dvi from ${IN}/fedata.pamphlet 
	@${DOCUMENT} dvi fedata ${DOC} fedata ${NOISE}


${MID}/FreeListProblems.h: ${IN}/freelistproblems.pamphlet
	@echo 1 making ${MID}/FreeListProblems.h from ${IN}/freelistproblems.pamphlet
	@${DOCUMENT} chunk "FreeListProblems.h" freelistproblems ${MID} FreeListProblems.h

${MID}/freelistproblems.C: ${IN}/freelistproblems.pamphlet
	@echo 2 making ${MID}/freelistproblems.C from ${IN}/freelistproblems.pamphlet
	@${DOCUMENT} src freelistproblems ${MID} freelistproblems.C

${OUT}/freelistproblems.o: ${MID}/freelistproblems.C
	@echo 3 making ${OUT}/freelistproblems.o from ${MID}/freelistproblems.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/freelistproblems.o ${MID}/freelistproblems.C

${DOC}/freelistproblems.dvi: ${IN}/freelistproblems.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/freelistproblems.dvi from ${IN}/freelistproblems.pamphlet 
	@${DOCUMENT} dvi freelistproblems ${DOC} freelistproblems ${NOISE}


${MID}/GACPforORGSolver.h: ${IN}/gacpfororgsolver.pamphlet
	@echo 1 making ${MID}/GACPforORGSolver.h from ${IN}/gacpfororgsolver.pamphlet
	@${DOCUMENT} chunk "GACPforORGSolver.h" gacpfororgsolver ${MID} GACPforORGSolver.h

${MID}/gacpfororgsolver.C: ${IN}/gacpfororgsolver.pamphlet
	@echo 2 making ${MID}/gacpfororgsolver.C from ${IN}/gacpfororgsolver.pamphlet
	@${DOCUMENT} src gacpfororgsolver ${MID} gacpfororgsolver.C

${OUT}/gacpfororgsolver.o: ${MID}/gacpfororgsolver.C
	@echo 3 making ${OUT}/gacpfororgsolver.o from ${MID}/gacpfororgsolver.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/gacpfororgsolver.o ${MID}/gacpfororgsolver.C

${DOC}/gacpfororgsolver.dvi: ${IN}/gacpfororgsolver.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/gacpfororgsolver.dvi from ${IN}/gacpfororgsolver.pamphlet 
	@${DOCUMENT} dvi gacpfororgsolver ${DOC} gacpfororgsolver ${NOISE}


${MID}/GAEquationSolver.h: ${IN}/gaequationsolver.pamphlet
	@echo 1 making ${MID}/GAEquationSolver.h from ${IN}/gaequationsolver.pamphlet
	@${DOCUMENT} chunk "GAEquationSolver.h" gaequationsolver ${MID} GAEquationSolver.h

${MID}/gaequationsolver.C: ${IN}/gaequationsolver.pamphlet
	@echo 2 making ${MID}/gaequationsolver.C from ${IN}/gaequationsolver.pamphlet
	@${DOCUMENT} src gaequationsolver ${MID} gaequationsolver.C

${OUT}/gaequationsolver.o: ${MID}/gaequationsolver.C
	@echo 3 making ${OUT}/gaequationsolver.o from ${MID}/gaequationsolver.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/gaequationsolver.o ${MID}/gaequationsolver.C

${DOC}/gaequationsolver.dvi: ${IN}/gaequationsolver.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/gaequationsolver.dvi from ${IN}/gaequationsolver.pamphlet 
	@${DOCUMENT} dvi gaequationsolver ${DOC} gaequationsolver ${NOISE}


${MID}/GAIsPartOfBasis.h: ${IN}/gaispartofbasis.pamphlet
	@echo 1 making ${MID}/GAIsPartOfBasis.h from ${IN}/gaispartofbasis.pamphlet
	@${DOCUMENT} chunk "GAIsPartOfBasis.h" gaispartofbasis ${MID} GAIsPartOfBasis.h

${MID}/gaispartofbasis.C: ${IN}/gaispartofbasis.pamphlet
	@echo 2 making ${MID}/gaispartofbasis.C from ${IN}/gaispartofbasis.pamphlet
	@${DOCUMENT} src gaispartofbasis ${MID} gaispartofbasis.C

${OUT}/gaispartofbasis.o: ${MID}/gaispartofbasis.C
	@echo 3 making ${OUT}/gaispartofbasis.o from ${MID}/gaispartofbasis.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/gaispartofbasis.o ${MID}/gaispartofbasis.C

${DOC}/gaispartofbasis.dvi: ${IN}/gaispartofbasis.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/gaispartofbasis.dvi from ${IN}/gaispartofbasis.pamphlet 
	@${DOCUMENT} dvi gaispartofbasis ${DOC} gaispartofbasis ${NOISE}


${MID}/GAWP.h: ${IN}/gawp.pamphlet
	@echo 1 making ${MID}/GAWP.h from ${IN}/gawp.pamphlet
	@${DOCUMENT} chunk "GAWP.h" gawp ${MID} GAWP.h

${MID}/gawp.C: ${IN}/gawp.pamphlet
	@echo 2 making ${MID}/gawp.C from ${IN}/gawp.pamphlet
	@${DOCUMENT} src gawp ${MID} gawp.C

${OUT}/gawp.o: ${MID}/gawp.C
	@echo 3 making ${OUT}/gawp.o from ${MID}/gawp.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/gawp.o ${MID}/gawp.C

${DOC}/gawp.dvi: ${IN}/gawp.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/gawp.dvi from ${IN}/gawp.pamphlet 
	@${DOCUMENT} dvi gawp ${DOC} gawp ${NOISE}


${MID}/HomEnumerators.h: ${IN}/homenumerators.pamphlet
	@echo 1 making ${MID}/HomEnumerators.h from ${IN}/homenumerators.pamphlet
	@${DOCUMENT} chunk "HomEnumerators.h" homenumerators ${MID} HomEnumerators.h

${MID}/homenumerators.C: ${IN}/homenumerators.pamphlet
	@echo 2 making ${MID}/homenumerators.C from ${IN}/homenumerators.pamphlet
	@${DOCUMENT} src homenumerators ${MID} homenumerators.C

${OUT}/homenumerators.o: ${MID}/homenumerators.C
	@echo 3 making ${OUT}/homenumerators.o from ${MID}/homenumerators.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/homenumerators.o ${MID}/homenumerators.C

${DOC}/homenumerators.dvi: ${IN}/homenumerators.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/homenumerators.dvi from ${IN}/homenumerators.pamphlet 
	@${DOCUMENT} dvi homenumerators ${DOC} homenumerators ${NOISE}


${MID}/ListProblems.h: ${IN}/listproblems.pamphlet
	@echo 1 making ${MID}/ListProblems.h from ${IN}/listproblems.pamphlet
	@${DOCUMENT} chunk "ListProblems.h" listproblems ${MID} ListProblems.h

${MID}/listproblems.C: ${IN}/listproblems.pamphlet
	@echo 2 making ${MID}/listproblems.C from ${IN}/listproblems.pamphlet
	@${DOCUMENT} src listproblems ${MID} listproblems.C

${OUT}/listproblems.o: ${MID}/listproblems.C
	@echo 3 making ${OUT}/listproblems.o from ${MID}/listproblems.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/listproblems.o ${MID}/listproblems.C

${DOC}/listproblems.dvi: ${IN}/listproblems.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/listproblems.dvi from ${IN}/listproblems.pamphlet 
	@${DOCUMENT} dvi listproblems ${DOC} listproblems ${NOISE}


${MID}/MakeRandomPresentation.h: ${IN}/makerandompresentation.pamphlet
	@echo 1 making ${MID}/MakeRandomPresentation.h from ${IN}/makerandompresentation.pamphlet
	@${DOCUMENT} chunk "MakeRandomPresentation.h" makerandompresentation ${MID} MakeRandomPresentation.h

${MID}/makerandompresentation.C: ${IN}/makerandompresentation.pamphlet
	@echo 2 making ${MID}/makerandompresentation.C from ${IN}/makerandompresentation.pamphlet
	@${DOCUMENT} src makerandompresentation ${MID} makerandompresentation.C

${OUT}/makerandompresentation.o: ${MID}/makerandompresentation.C
	@echo 3 making ${OUT}/makerandompresentation.o from ${MID}/makerandompresentation.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/makerandompresentation.o ${MID}/makerandompresentation.C

${DOC}/makerandompresentation.dvi: ${IN}/makerandompresentation.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/makerandompresentation.dvi from ${IN}/makerandompresentation.pamphlet 
	@${DOCUMENT} dvi makerandompresentation ${DOC} makerandompresentation ${NOISE}


${MID}/magnus.h: ${IN}/magnus.pamphlet
	@echo 13 making ${MID}/magnus.h from ${IN}/magnus.pamphlet
	@${DOCUMENT} chunk "magnus.h" magnus ${MID} magnus.h

${MID}/magnus.C: ${IN}/magnus.pamphlet
	@echo 14 making ${MID}/magnus.C from ${IN}/magnus.pamphlet
	@${DOCUMENT} src magnus ${MID} magnus.C

${OUT}/magnus.o: ${MID}/magnus.C
	@echo 15 making ${OUT}/magnus.o from ${MID}/magnus.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/magnus.o ${MID}/magnus.C

${DOC}/magnus.dvi: ${IN}/magnus.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/magnus.dvi from ${IN}/magnus.pamphlet 
	@${DOCUMENT} dvi magnus ${DOC} magnus ${NOISE}

${BIN}/magnus: ${OUT}/magnus.o
	@echo 17 making ${BIN}/magnus from ${OUT}/magnus.o
	@(cd ${OUT} ; \
	gcc ${CCFLAGS} ${INCLUDE} ${OFILES} -o magnus ${OUT}/magnus.o -lstdc++ ; \
	cp magnus ${BIN} )


${MID}/MagnusHome.h: ${IN}/magnushome.pamphlet
	@echo 1 making ${MID}/MagnusHome.h from ${IN}/magnushome.pamphlet
	@${DOCUMENT} chunk "MagnusHome.h" magnushome ${MID} MagnusHome.h

${MID}/magnushome.C: ${IN}/magnushome.pamphlet
	@echo 2 making ${MID}/magnushome.C from ${IN}/magnushome.pamphlet
	@${DOCUMENT} src magnushome ${MID} magnushome.C

${OUT}/magnushome.o: ${MID}/magnushome.C
	@echo 3 making ${OUT}/magnushome.o from ${MID}/magnushome.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/magnushome.o ${MID}/magnushome.C

${DOC}/magnushome.dvi: ${IN}/magnushome.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/magnushome.dvi from ${IN}/magnushome.pamphlet 
	@${DOCUMENT} dvi magnushome ${DOC} magnushome ${NOISE}


${MID}/menudefns.C: ${IN}/menudefns.pamphlet
	@echo 2 making ${MID}/menudefns.C from ${IN}/menudefns.pamphlet
	@${DOCUMENT} src menudefns ${MID} menudefns.C

${OUT}/menudefns.o: ${MID}/menudefns.C
	@echo 3 making ${OUT}/menudefns.o from ${MID}/menudefns.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/menudefns.o ${MID}/menudefns.C

${DOC}/menudefns.dvi: ${IN}/menudefns.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/menudefns.dvi from ${IN}/menudefns.pamphlet 
	@${DOCUMENT} dvi menudefns ${DOC} menudefns ${NOISE}


${MID}/NormalRandomWord.h: ${IN}/normalrandomword.pamphlet
	@echo 1 making ${MID}/NormalRandomWord.h from ${IN}/normalrandomword.pamphlet
	@${DOCUMENT} chunk "NormalRandomWord.h" normalrandomword ${MID} NormalRandomWord.h

${MID}/normalrandomword.C: ${IN}/normalrandomword.pamphlet
	@echo 2 making ${MID}/normalrandomword.C from ${IN}/normalrandomword.pamphlet
	@${DOCUMENT} src normalrandomword ${MID} normalrandomword.C

${OUT}/normalrandomword.o: ${MID}/normalrandomword.C
	@echo 3 making ${OUT}/normalrandomword.o from ${MID}/normalrandomword.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/normalrandomword.o ${MID}/normalrandomword.C

${DOC}/normalrandomword.dvi: ${IN}/normalrandomword.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/normalrandomword.dvi from ${IN}/normalrandomword.pamphlet 
	@${DOCUMENT} dvi normalrandomword ${DOC} normalrandomword ${NOISE}


${MID}/ObjectSmith.h: ${IN}/objectsmith.pamphlet
	@echo 18 making ${MID}/ObjectSmith.h from ${IN}/objectsmith.pamphlet
	@${DOCUMENT} chunk "ObjectSmith.h" objectsmith ${MID} ObjectSmith.h

${MID}/objectsmith.C: ${IN}/objectsmith.pamphlet
	@echo 19 making ${MID}/objectsmith.C from ${IN}/objectsmith.pamphlet
	@${DOCUMENT} src objectsmith ${MID} objectsmith.C

${OUT}/objectsmith.o: ${MID}/objectsmith.C
	@echo 20 making ${OUT}/objectsmith.o from ${MID}/objectsmith.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/objectsmith.o ${MID}/objectsmith.C

${DOC}/objectsmith.dvi: ${IN}/objectsmith.pamphlet ${DOC}/axiom.sty
	@echo 21 making ${DOC}/objectsmith.dvi from ${IN}/objectsmith.pamphlet 
	@${DOCUMENT} dvi objectsmith ${DOC} objectsmith ${NOISE}


${MID}/ORConsequencesEnumerator.h: ${IN}/orconsequencesenumerator.pamphlet
	@echo 22 making ${MID}/ORConsequencesEnumerator.h from ${IN}/orconsequencesenumerator.pamphlet
	@${DOCUMENT} chunk "ORConsequencesEnumerator.h" orconsequencesenumerator ${MID} ORConsequencesEnumerator.h

${MID}/orconsequencesenumerator.C: ${IN}/orconsequencesenumerator.pamphlet
	@echo 23 making ${MID}/orconsequencesenumerator.C from ${IN}/orconsequencesenumerator.pamphlet
	@${DOCUMENT} src orconsequencesenumerator ${MID} orconsequencesenumerator.C

${OUT}/orconsequencesenumerator.o: ${MID}/orconsequencesenumerator.C
	@echo 24 making ${OUT}/orconsequencesenumerator.o from ${MID}/orconsequencesenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/orconsequencesenumerator.o ${MID}/orconsequencesenumerator.C

${DOC}/orconsequencesenumerator.dvi: ${IN}/orconsequencesenumerator.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/orconsequencesenumerator.dvi from ${IN}/orconsequencesenumerator.pamphlet 
	@${DOCUMENT} dvi orconsequencesenumerator ${DOC} orconsequencesenumerator ${NOISE}


${MID}/OutMessages.h: ${IN}/outmessages.pamphlet
	@echo 22 making ${MID}/OutMessages.h from ${IN}/outmessages.pamphlet
	@${DOCUMENT} chunk "OutMessages.h" outmessages ${MID} OutMessages.h

${MID}/outmessages.C: ${IN}/outmessages.pamphlet
	@echo 23 making ${MID}/outmessages.C from ${IN}/outmessages.pamphlet
	@${DOCUMENT} src outmessages ${MID} outmessages.C

${OUT}/outmessages.o: ${MID}/outmessages.C
	@echo 24 making ${OUT}/outmessages.o from ${MID}/outmessages.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/outmessages.o ${MID}/outmessages.C

${DOC}/outmessages.dvi: ${IN}/outmessages.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/outmessages.dvi from ${IN}/outmessages.pamphlet 
	@${DOCUMENT} dvi outmessages ${DOC} outmessages ${NOISE}


${MID}/PackagesData.h: ${IN}/packagesdata.pamphlet
	@echo 1 making ${MID}/PackagesData.h from ${IN}/packagesdata.pamphlet
	@${DOCUMENT} chunk "PackagesData.h" packagesdata ${MID} PackagesData.h

${MID}/packagesdata.C: ${IN}/packagesdata.pamphlet
	@echo 2 making ${MID}/packagesdata.C from ${IN}/packagesdata.pamphlet
	@${DOCUMENT} src packagesdata ${MID} packagesdata.C

${OUT}/packagesdata.o: ${MID}/packagesdata.C
	@echo 3 making ${OUT}/packagesdata.o from ${MID}/packagesdata.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/packagesdata.o ${MID}/packagesdata.C

${DOC}/packagesdata.dvi: ${IN}/packagesdata.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/packagesdata.dvi from ${IN}/packagesdata.pamphlet 
	@${DOCUMENT} dvi packagesdata ${DOC} packagesdata ${NOISE}


${MID}/PackagesSMApps.h: ${IN}/packagessmapps.pamphlet
	@echo 1 making ${MID}/PackagesSMApps.h from ${IN}/packagessmapps.pamphlet
	@${DOCUMENT} chunk "PackagesSMApps.h" packagessmapps ${MID} PackagesSMApps.h

${MID}/packagessmapps.C: ${IN}/packagessmapps.pamphlet
	@echo 2 making ${MID}/packagessmapps.C from ${IN}/packagessmapps.pamphlet
	@${DOCUMENT} src packagessmapps ${MID} packagessmapps.C

${OUT}/packagessmapps.o: ${MID}/packagessmapps.C
	@echo 3 making ${OUT}/packagessmapps.o from ${MID}/packagessmapps.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/packagessmapps.o ${MID}/packagessmapps.C

${DOC}/packagessmapps.dvi: ${IN}/packagessmapps.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/packagessmapps.dvi from ${IN}/packagessmapps.pamphlet 
	@${DOCUMENT} dvi packagessmapps ${DOC} packagessmapps ${NOISE}


${MID}/PresentationProblems.h: ${IN}/presentationproblems.pamphlet
	@echo 1 making ${MID}/PresentationProblems.h from ${IN}/presentationproblems.pamphlet
	@${DOCUMENT} chunk "PresentationProblems.h" presentationproblems ${MID} PresentationProblems.h

${MID}/presentationproblems.C: ${IN}/presentationproblems.pamphlet
	@echo 2 making ${MID}/presentationproblems.C from ${IN}/presentationproblems.pamphlet
	@${DOCUMENT} src presentationproblems ${MID} presentationproblems.C

${OUT}/presentationproblems.o: ${MID}/presentationproblems.C
	@echo 3 making ${OUT}/presentationproblems.o from ${MID}/presentationproblems.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/presentationproblems.o ${MID}/presentationproblems.C

${DOC}/presentationproblems.dvi: ${IN}/presentationproblems.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/presentationproblems.dvi from ${IN}/presentationproblems.pamphlet 
	@${DOCUMENT} dvi presentationproblems ${DOC} presentationproblems ${NOISE}


${MID}/registerproperties.C: ${IN}/registerproperties.pamphlet
	@echo 2 making ${MID}/registerproperties.C from ${IN}/registerproperties.pamphlet
	@${DOCUMENT} src registerproperties ${MID} registerproperties.C

${OUT}/registerproperties.o: ${MID}/registerproperties.C
	@echo 3 making ${OUT}/registerproperties.o from ${MID}/registerproperties.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/registerproperties.o ${MID}/registerproperties.C

${DOC}/registerproperties.dvi: ${IN}/registerproperties.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/registerproperties.dvi from ${IN}/registerproperties.pamphlet 
	@${DOCUMENT} dvi registerproperties ${DOC} registerproperties ${NOISE}


${MID}/REnumerator.h: ${IN}/renumerator.pamphlet
	@echo 30 making ${MID}/REnumerator.h from ${IN}/renumerator.pamphlet
	@${DOCUMENT} chunk "REnumerator.h" renumerator ${MID} REnumerator.h

${MID}/renumerator.C: ${IN}/renumerator.pamphlet
	@echo 31 making ${MID}/renumerator.C from ${IN}/renumerator.pamphlet
	@${DOCUMENT} src renumerator ${MID} renumerator.C

${OUT}/renumerator.o: ${MID}/renumerator.C
	@echo 32 making ${OUT}/renumerator.o from ${MID}/renumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/renumerator.o ${MID}/renumerator.C

${DOC}/renumerator.dvi: ${IN}/renumerator.pamphlet ${DOC}/axiom.sty
	@echo 33 making ${DOC}/renumerator.dvi from ${IN}/renumerator.pamphlet 
	@${DOCUMENT} dvi renumerator ${DOC} renumerator ${NOISE}


${MID}/Roulette.h: ${IN}/roulette.pamphlet
	@echo 1 making ${MID}/Roulette.h from ${IN}/roulette.pamphlet
	@${DOCUMENT} chunk "Roulette.h" roulette ${MID} Roulette.h

${DOC}/roulette.dvi: ${IN}/roulette.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/roulette.dvi from ${IN}/roulette.pamphlet 
	@${DOCUMENT} dvi roulette ${DOC} roulette ${NOISE}


${MID}/SessionManager.h: ${IN}/sessionmanager.pamphlet
	@echo 22 making ${MID}/SessionManager.h from ${IN}/sessionmanager.pamphlet
	@${DOCUMENT} chunk "SessionManager.h" sessionmanager ${MID} SessionManager.h

${MID}/sessionmanager.C: ${IN}/sessionmanager.pamphlet
	@echo 23 making ${MID}/sessionmanager.C from ${IN}/sessionmanager.pamphlet
	@${DOCUMENT} src sessionmanager ${MID} sessionmanager.C

${OUT}/sessionmanager.o: ${MID}/sessionmanager.C
	@echo 24 making ${OUT}/sessionmanager.o from ${MID}/sessionmanager.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/sessionmanager.o ${MID}/sessionmanager.C

${DOC}/sessionmanager.dvi: ${IN}/sessionmanager.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/sessionmanager.dvi from ${IN}/sessionmanager.pamphlet 
	@${DOCUMENT} dvi sessionmanager ${DOC} sessionmanager ${NOISE}


${MID}/Set.h: ${IN}/set.pamphlet
	@echo 22 making ${MID}/Set.h from ${IN}/set.pamphlet
	@${DOCUMENT} chunk "Set.h" set ${MID} Set.h

${MID}/set.C: ${IN}/set.pamphlet
	@echo 23 making ${MID}/set.C from ${IN}/set.pamphlet
	@${DOCUMENT} src set ${MID} set.C

${OUT}/set.o: ${MID}/set.C
	@echo 24 making ${OUT}/set.o from ${MID}/set.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/set.o ${MID}/set.C

${DOC}/set.dvi: ${IN}/set.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/set.dvi from ${IN}/set.pamphlet 
	@${DOCUMENT} dvi set ${DOC} set ${NOISE}


${MID}/SGREnumerator.h: ${IN}/sgrenumerator.pamphlet
	@echo 22 making ${MID}/SGREnumerator.h from ${IN}/sgrenumerator.pamphlet
	@${DOCUMENT} chunk "SGREnumerator.h" sgrenumerator ${MID} SGREnumerator.h

${MID}/sgrenumerator.C: ${IN}/sgrenumerator.pamphlet
	@echo 23 making ${MID}/sgrenumerator.C from ${IN}/sgrenumerator.pamphlet
	@${DOCUMENT} src sgrenumerator ${MID} sgrenumerator.C

${OUT}/sgrenumerator.o: ${MID}/sgrenumerator.C
	@echo 24 making ${OUT}/sgrenumerator.o from ${MID}/sgrenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/sgrenumerator.o ${MID}/sgrenumerator.C

${DOC}/sgrenumerator.dvi: ${IN}/sgrenumerator.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/sgrenumerator.dvi from ${IN}/sgrenumerator.pamphlet 
	@${DOCUMENT} dvi sgrenumerator ${DOC} sgrenumerator ${NOISE}


${MID}/SMEnumerator.h: ${IN}/smenumerator.pamphlet
	@echo 22 making ${MID}/SMEnumerator.h from ${IN}/smenumerator.pamphlet
	@${DOCUMENT} chunk "SMEnumerator.h" smenumerator ${MID} SMEnumerator.h

${MID}/smenumerator.C: ${IN}/smenumerator.pamphlet
	@echo 23 making ${MID}/smenumerator.C from ${IN}/smenumerator.pamphlet
	@${DOCUMENT} src smenumerator ${MID} smenumerator.C

${OUT}/smenumerator.o: ${MID}/smenumerator.C
	@echo 24 making ${OUT}/smenumerator.o from ${MID}/smenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smenumerator.o ${MID}/smenumerator.C

${DOC}/smenumerator.dvi: ${IN}/smenumerator.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/smenumerator.dvi from ${IN}/smenumerator.pamphlet 
	@${DOCUMENT} dvi smenumerator ${DOC} smenumerator ${NOISE}


${MID}/SMList.h: ${IN}/smlist.pamphlet
	@echo 1 making ${MID}/SMList.h from ${IN}/smlist.pamphlet
	@${DOCUMENT} chunk "SMList.h" smlist ${MID} SMList.h

${MID}/smlist.C: ${IN}/smlist.pamphlet
	@echo 2 making ${MID}/smlist.C from ${IN}/smlist.pamphlet
	@${DOCUMENT} src smlist ${MID} smlist.C

${OUT}/smlist.o: ${MID}/smlist.C
	@echo 3 making ${OUT}/smlist.o from ${MID}/smlist.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smlist.o ${MID}/smlist.C

${DOC}/smlist.dvi: ${IN}/smlist.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/smlist.dvi from ${IN}/smlist.pamphlet 
	@${DOCUMENT} dvi smlist ${DOC} smlist ${NOISE}


${MID}/SMListIterator.h: ${IN}/smlistiterator.pamphlet
	@echo 22 making ${MID}/SMListIterator.h from ${IN}/smlistiterator.pamphlet
	@${DOCUMENT} chunk "SMListIterator.h" smlistiterator ${MID} SMListIterator.h

${MID}/smlistiterator.C: ${IN}/smlistiterator.pamphlet
	@echo 23 making ${MID}/smlistiterator.C from ${IN}/smlistiterator.pamphlet
	@${DOCUMENT} src smlistiterator ${MID} smlistiterator.C

${OUT}/smlistiterator.o: ${MID}/smlistiterator.C
	@echo 24 making ${OUT}/smlistiterator.o from ${MID}/smlistiterator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smlistiterator.o ${MID}/smlistiterator.C

${DOC}/smlistiterator.dvi: ${IN}/smlistiterator.pamphlet ${DOC}/axiom.sty
	@echo 25 making ${DOC}/smlistiterator.dvi from ${IN}/smlistiterator.pamphlet 
	@${DOCUMENT} dvi smlistiterator ${DOC} smlistiterator ${NOISE}


${MID}/SMListSubgroupProblems.h: ${IN}/smlistsubgroupproblems.pamphlet
	@echo 1 making ${MID}/SMListSubgroupProblems.h from ${IN}/smlistsubgroupproblems.pamphlet
	@${DOCUMENT} chunk "SMListSubgroupProblems.h" smlistsubgroupproblems ${MID} SMListSubgroupProblems.h

${MID}/smlistsubgroupproblems.C: ${IN}/smlistsubgroupproblems.pamphlet
	@echo 2 making ${MID}/smlistsubgroupproblems.C from ${IN}/smlistsubgroupproblems.pamphlet
	@${DOCUMENT} src smlistsubgroupproblems ${MID} smlistsubgroupproblems.C

${OUT}/smlistsubgroupproblems.o: ${MID}/smlistsubgroupproblems.C
	@echo 3 making ${OUT}/smlistsubgroupproblems.o from ${MID}/smlistsubgroupproblems.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smlistsubgroupproblems.o ${MID}/smlistsubgroupproblems.C

${DOC}/smlistsubgroupproblems.dvi: ${IN}/smlistsubgroupproblems.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/smlistsubgroupproblems.dvi from ${IN}/smlistsubgroupproblems.pamphlet 
	@${DOCUMENT} dvi smlistsubgroupproblems ${DOC} smlistsubgroupproblems ${NOISE}


${MID}/SMListWordProblem.h: ${IN}/smlistwordproblem.pamphlet
	@echo 1 making ${MID}/SMListWordProblem.h from ${IN}/smlistwordproblem.pamphlet
	@${DOCUMENT} chunk "SMListWordProblem.h" smlistwordproblem ${MID} SMListWordProblem.h

${MID}/smlistwordproblem.C: ${IN}/smlistwordproblem.pamphlet
	@echo 2 making ${MID}/smlistwordproblem.C from ${IN}/smlistwordproblem.pamphlet
	@${DOCUMENT} src smlistwordproblem ${MID} smlistwordproblem.C

${OUT}/smlistwordproblem.o: ${MID}/smlistwordproblem.C
	@echo 3 making ${OUT}/smlistwordproblem.o from ${MID}/smlistwordproblem.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smlistwordproblem.o ${MID}/smlistwordproblem.C

${DOC}/smlistwordproblem.dvi: ${IN}/smlistwordproblem.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/smlistwordproblem.dvi from ${IN}/smlistwordproblem.pamphlet 
	@${DOCUMENT} dvi smlistwordproblem ${DOC} smlistwordproblem ${NOISE}


${MID}/SMMap.h: ${IN}/smmap.pamphlet
	@echo 1 making ${MID}/SMMap.h from ${IN}/smmap.pamphlet
	@${DOCUMENT} chunk "SMMap.h" smmap ${MID} SMMap.h

${MID}/smmap.C: ${IN}/smmap.pamphlet
	@echo 2 making ${MID}/smmap.C from ${IN}/smmap.pamphlet
	@${DOCUMENT} src smmap ${MID} smmap.C

${OUT}/smmap.o: ${MID}/smmap.C
	@echo 3 making ${OUT}/smmap.o from ${MID}/smmap.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/smmap.o ${MID}/smmap.C

${DOC}/smmap.dvi: ${IN}/smmap.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/smmap.dvi from ${IN}/smmap.pamphlet 
	@${DOCUMENT} dvi smmap ${DOC} smmap ${NOISE}


${MID}/SubgroupEnumerator.h: ${IN}/subgroupenumerator.pamphlet
	@echo 1 making ${MID}/SubgroupEnumerator.h from ${IN}/subgroupenumerator.pamphlet
	@${DOCUMENT} chunk "SubgroupEnumerator.h" subgroupenumerator ${MID} SubgroupEnumerator.h

${MID}/subgroupenumerator.C: ${IN}/subgroupenumerator.pamphlet
	@echo 2 making ${MID}/subgroupenumerator.C from ${IN}/subgroupenumerator.pamphlet
	@${DOCUMENT} src subgroupenumerator ${MID} subgroupenumerator.C

${OUT}/subgroupenumerator.o: ${MID}/subgroupenumerator.C
	@echo 3 making ${OUT}/subgroupenumerator.o from ${MID}/subgroupenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/subgroupenumerator.o ${MID}/subgroupenumerator.C

${DOC}/subgroupenumerator.dvi: ${IN}/subgroupenumerator.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/subgroupenumerator.dvi from ${IN}/subgroupenumerator.pamphlet 
	@${DOCUMENT} dvi subgroupenumerator ${DOC} subgroupenumerator ${NOISE}


${MID}/TheObjects.h: ${IN}/theobjects.pamphlet
	@echo 26 making ${MID}/TheObjects.h from ${IN}/theobjects.pamphlet
	@${DOCUMENT} chunk "TheObjects.h" theobjects ${MID} TheObjects.h

${MID}/theobjects.C: ${IN}/theobjects.pamphlet
	@echo 27 making ${MID}/theobjects.C from ${IN}/theobjects.pamphlet
	@${DOCUMENT} src theobjects ${MID} theobjects.C

${OUT}/theobjects.o: ${MID}/theobjects.C
	@echo 28 making ${OUT}/theobjects.o from ${MID}/theobjects.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/theobjects.o ${MID}/theobjects.C

${DOC}/theobjects.dvi: ${IN}/theobjects.pamphlet ${DOC}/axiom.sty
	@echo 29 making ${DOC}/theobjects.dvi from ${IN}/theobjects.pamphlet 
	@${DOCUMENT} dvi theobjects ${DOC} theobjects ${NOISE}


${MID}/WEnumerator.h: ${IN}/wenumerator.pamphlet
	@echo 30 making ${MID}/WEnumerator.h from ${IN}/wenumerator.pamphlet
	@${DOCUMENT} chunk "WEnumerator.h" wenumerator ${MID} WEnumerator.h

${MID}/wenumerator.C: ${IN}/wenumerator.pamphlet
	@echo 31 making ${MID}/wenumerator.C from ${IN}/wenumerator.pamphlet
	@${DOCUMENT} src wenumerator ${MID} wenumerator.C

${OUT}/wenumerator.o: ${MID}/wenumerator.C
	@echo 32 making ${OUT}/wenumerator.o from ${MID}/wenumerator.C
	@g++ ${CCFLAGS} ${INCLUDE} -c -o ${OUT}/wenumerator.o ${MID}/wenumerator.C

${DOC}/wenumerator.dvi: ${IN}/wenumerator.pamphlet ${DOC}/axiom.sty
	@echo 33 making ${DOC}/wenumerator.dvi from ${IN}/wenumerator.pamphlet 
	@${DOCUMENT} dvi wenumerator ${DOC} wenumerator ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 30 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
