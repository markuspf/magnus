
ENV= MAG=${MAG} SYS=${SYS} SRC=${SRC} INC=${INC} INT=${INT} OBJ=${OBJ} \
     TMP=${TMP} BIN=${BIN} MNT=${MNT} ZIPS=${ZIPS} \
     CCFLAGS='${CCFLAGS}' INCLUDE='${INCLUDE}' NOISE=${NOISE} \
     DOCUMENT=${DOCUMENT} TANGLE=${TANGLE} WEAVE=${WEAVE}

INCDIRS=AProductsdir Equationsdir FSAdir GAPdir KBdir globaldir generaldir \
        Groupdir libg++dir Mapdir Matrixdir NilpotentGroupdir \
        Packagesdir Polynomialdir SMAppsdir Statisticsdir Subgroupdir \
        Todd-Coxeterdir

CODEDIRS=SessionManagerdir 

all: ${INCDIRS} ${CODEDIRS}
	@echo 14 finished ${SRC}/backend

AProductsdir:
	@echo 1 making ${SRC}/backend/AProducts
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/AProducts
	@mkdir -p ${OBJ}/${SYS}/backend/AProducts
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/AProducts
	@(cd AProducts ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Equationsdir:
	@echo 1 making ${SRC}/backend/Equations
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Equations
	@mkdir -p ${OBJ}/${SYS}/backend/Equations
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Equations
	@(cd Equations ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

FSAdir:
	@echo 2 making ${SRC}/backend/FSA
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/FSA
	@mkdir -p ${OBJ}/${SYS}/backend/FSA
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/FSA
	@(cd FSA ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

GAPdir:
	@echo 1 making ${SRC}/backend/GAP
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/GAP
	@mkdir -p ${OBJ}/${SYS}/backend/GAP
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/GAP
	@(cd GAP ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

KBdir:
	@echo 3 making ${SRC}/backend/KB
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/KB
	@mkdir -p ${OBJ}/${SYS}/backend/KB
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/KB
	@(cd KB ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

globaldir:
	@echo 5 making ${SRC}/backend/global
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/global
	@mkdir -p ${OBJ}/${SYS}/backend/global
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/global
	@(cd global ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

generaldir:
	@echo 4 making ${SRC}/backend/general
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/general
	@mkdir -p ${OBJ}/${SYS}/backend/general
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/general
	@(cd general ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Groupdir:
	@echo 6 making ${SRC}/backend/Group
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Group
	@mkdir -p ${OBJ}/${SYS}/backend/Group
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Group
	@(cd Group ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

libg++dir:
	@echo 7 making ${SRC}/backend/libg++
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/libg++
	@mkdir -p ${OBJ}/${SYS}/backend/libg++
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/libg++
	@(cd libg++ ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Mapdir:
	@echo 8 making ${SRC}/backend/Map
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Map
	@mkdir -p ${OBJ}/${SYS}/backend/Map
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Map
	@(cd Map ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Matrixdir:
	@echo 9 making ${SRC}/backend/Matrix
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Matrix
	@mkdir -p ${OBJ}/${SYS}/backend/Matrix
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Matrix
	@(cd Matrix ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

NilpotentGroupdir:
	@echo 10 making ${SRC}/backend/NilpotentGroup
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/NilpotentGroup
	@mkdir -p ${OBJ}/${SYS}/backend/NilpotentGroup
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/NilpotentGroup
	@(cd NilpotentGroup ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Packagesdir:
	@echo 3 making ${SRC}/backend/Packages
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Packages
	@mkdir -p ${OBJ}/${SYS}/backend/Packages
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Packages
	@(cd Packages ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Polynomialdir:
	@echo 1 making ${SRC}/backend/Polynomial
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Polynomial
	@mkdir -p ${OBJ}/${SYS}/backend/Polynomial
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Polynomial
	@(cd Polynomial ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

SessionManagerdir:
	@echo 11 making ${SRC}/backend/SessionManager
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/SessionManager
	@mkdir -p ${OBJ}/${SYS}/backend/SessionManager
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/SessionManager
	@(cd SessionManager ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

SMAppsdir:
	@echo 1 making ${SRC}/backend/SMApps
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/SMApps
	@mkdir -p ${OBJ}/${SYS}/backend/SMApps
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/SMApps
	@(cd SMApps ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Statisticsdir:
	@echo 3 making ${SRC}/backend/Statistics
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Statistics
	@mkdir -p ${OBJ}/${SYS}/backend/Statistics
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Statistics
	@(cd Statistics ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Subgroupdir:
	@echo 12 making ${SRC}/backend/Subgroup
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Subgroup
	@mkdir -p ${OBJ}/${SYS}/backend/Subgroup
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Subgroup
	@(cd Subgroup ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )

Todd-Coxeterdir:
	@echo 13 making ${SRC}/backend/Todd-Coxeter
	@mkdir -p ${INT}/include
	@mkdir -p ${INT}/backend/Todd-Coxeter
	@mkdir -p ${OBJ}/${SYS}/backend/Todd-Coxeter
	@mkdir -p ${MNT}/${SYS}/doc/src/backend/Todd-Coxeter
	@(cd Todd-Coxeter ; \
	 ${DOCUMENT} src Makefile ; \
	 ${DOCUMENT} dvi Makefile . Makefile ${NOISE} ; \
	 ${ENV} ${MAKE} )


