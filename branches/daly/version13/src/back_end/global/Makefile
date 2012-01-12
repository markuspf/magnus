IN=${SRC}/backend/global
INC=${INT}/include
DOC=${MNT}/${SYS}/doc/src/backend/global

FILES= ${INC}/BaseObjectOf.h ${INC}/config.h \
       ${INC}/DerivedObjectOf.h ${INC}/error.h \
       ${INC}/ExtendedIPC.h ${INC}/GenericObject.h \
       ${INC}/global.h ${INC}/IPC.h \
       ${INC}/Object.h ${INC}/ObjectOf.h \
       ${INC}/PureRep.h ${INC}/RefCounter.h \
       ${INC}/Trichotomy.h

DOCFILES= ${DOC}/BaseObjectOf.h.dvi ${DOC}/config.h.dvi \
          ${DOC}/DerivedObjectOf.h.dvi ${DOC}/error.h.dvi \
          ${DOC}/ExtendedIPC.h.dvi ${DOC}/GenericObject.h.dvi \
          ${DOC}/global.h.dvi ${DOC}/IPC.h.dvi \
          ${DOC}/Object.h.dvi ${DOC}/ObjectOf.h.dvi \
          ${DOC}/PureRep.h.dvi ${DOC}/RefCounter.h.dvi \
          ${DOC}/Trichotomy.h.dvi

all: ${FILES} ${DOCFILES} 
	@echo 28 finished ${IN}

${INC}/BaseObjectOf.h: ${IN}/BaseObjectOf.h.pamphlet
	@echo 1 making ${INC}/BaseObjectOf.h from ${IN}/BaseObjectOf.h.pamphlet
	@${DOCUMENT} src BaseObjectOf.h ${INC}

${DOC}/BaseObjectOf.h.dvi: ${IN}/BaseObjectOf.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/BaseObjectOf.h.dvi from ${IN}/BaseObjectOf.h.pamphlet
	@${DOCUMENT} dvi BaseObjectOf.h ${DOC} BaseObjectOf.h ${NOISE}


${INC}/config.h: ${IN}/config.h.pamphlet
	@echo 3 making ${INC}/config.h from ${IN}/config.h.pamphlet
	@${DOCUMENT} src config.h ${INC}

${DOC}/config.h.dvi: ${IN}/config.h.pamphlet ${DOC}/axiom.sty
	@echo 4 making ${DOC}/config.h.dvi from ${IN}/config.h.pamphlet
	@${DOCUMENT} dvi config.h ${DOC} config.h ${NOISE}


${INC}/DerivedObjectOf.h: ${IN}/DerivedObjectOf.h.pamphlet
	@echo 5 making ${INC}/DerivedObjectOf.h from ${IN}/DerivedObjectOf.h.pamphlet
	@${DOCUMENT} src DerivedObjectOf.h ${INC}

${DOC}/DerivedObjectOf.h.dvi: ${IN}/DerivedObjectOf.h.pamphlet ${DOC}/axiom.sty
	@echo 6 making ${DOC}/DerivedObjectOf.h.dvi from ${IN}/DerivedObjectOf.h.pamphlet
	@${DOCUMENT} dvi DerivedObjectOf.h ${DOC} DerivedObjectOf.h ${NOISE}


${INC}/error.h: ${IN}/error.h.pamphlet
	@echo 7 making ${INC}/error.h from ${IN}/error.h.pamphlet
	@${DOCUMENT} src error.h ${INC}

${DOC}/error.h.dvi: ${IN}/error.h.pamphlet ${DOC}/axiom.sty
	@echo 8 making ${DOC}/error.h.dvi from ${IN}/error.h.pamphlet
	@${DOCUMENT} dvi error.h ${DOC} error.h ${NOISE}


${INC}/ExtendedIPC.h: ${IN}/ExtendedIPC.h.pamphlet
	@echo 9 making ${INC}/ExtendedIPC.h from ${IN}/ExtendedIPC.h.pamphlet
	@${DOCUMENT} src ExtendedIPC.h ${INC}

${DOC}/ExtendedIPC.h.dvi: ${IN}/ExtendedIPC.h.pamphlet ${DOC}/axiom.sty
	@echo 10 making ${DOC}/ExtendedIPC.h.dvi from ${IN}/ExtendedIPC.h.pamphlet
	@${DOCUMENT} dvi ExtendedIPC.h ${DOC} ExtendedIPC.h ${NOISE}


${INC}/GenericObject.h: ${IN}/GenericObject.h.pamphlet
	@echo 11 making ${INC}/GenericObject.h from ${IN}/GenericObject.h.pamphlet
	@${DOCUMENT} src GenericObject.h ${INC}

${DOC}/GenericObject.h.dvi: ${IN}/GenericObject.h.pamphlet ${DOC}/axiom.sty
	@echo 12 making ${DOC}/GenericObject.h.dvi from ${IN}/GenericObject.h.pamphlet
	@${DOCUMENT} dvi GenericObject.h ${DOC} GenericObject.h ${NOISE}


${INC}/global.h: ${IN}/global.h.pamphlet
	@echo 13 making ${INC}/global.h from ${IN}/global.h.pamphlet
	@${DOCUMENT} src global.h ${INC}

${DOC}/global.h.dvi: ${IN}/global.h.pamphlet ${DOC}/axiom.sty
	@echo 14 making ${DOC}/global.h.dvi from ${IN}/global.h.pamphlet
	@${DOCUMENT} dvi global.h ${DOC} global.h ${NOISE}


${INC}/IPC.h: ${IN}/IPC.h.pamphlet
	@echo 15 making ${INC}/IPC.h from ${IN}/IPC.h.pamphlet
	@${DOCUMENT} src IPC.h ${INC}

${DOC}/IPC.h.dvi: ${IN}/IPC.h.pamphlet ${DOC}/axiom.sty
	@echo 16 making ${DOC}/IPC.h.dvi from ${IN}/IPC.h.pamphlet
	@${DOCUMENT} dvi IPC.h ${DOC} IPC.h ${NOISE}


${INC}/Object.h: ${IN}/Object.h.pamphlet
	@echo 17 making ${INC}/Object.h from ${IN}/Object.h.pamphlet
	@${DOCUMENT} src Object.h ${INC}

${DOC}/Object.h.dvi: ${IN}/Object.h.pamphlet ${DOC}/axiom.sty
	@echo 18 making ${DOC}/Object.h.dvi from ${IN}/Object.h.pamphlet
	@${DOCUMENT} dvi Object.h ${DOC} Object.h ${NOISE}


${INC}/ObjectOf.h: ${IN}/ObjectOf.h.pamphlet
	@echo 19 making ${INC}/ObjectOf.h from ${IN}/ObjectOf.h.pamphlet
	@${DOCUMENT} src ObjectOf.h ${INC}

${DOC}/ObjectOf.h.dvi: ${IN}/ObjectOf.h.pamphlet ${DOC}/axiom.sty
	@echo 20 making ${DOC}/ObjectOf.h.dvi from ${IN}/ObjectOf.h.pamphlet
	@${DOCUMENT} dvi ObjectOf.h ${DOC} ObjectOf.h ${NOISE}


${INC}/PureRep.h: ${IN}/PureRep.h.pamphlet
	@echo 21 making ${INC}/PureRep.h from ${IN}/PureRep.h.pamphlet
	@${DOCUMENT} src PureRep.h ${INC}

${DOC}/PureRep.h.dvi: ${IN}/PureRep.h.pamphlet ${DOC}/axiom.sty
	@echo 22 making ${DOC}/PureRep.h.dvi from ${IN}/PureRep.h.pamphlet
	@${DOCUMENT} dvi PureRep.h ${DOC} PureRep.h ${NOISE}


${INC}/RefCounter.h: ${IN}/RefCounter.h.pamphlet
	@echo 23 making ${INC}/RefCounter.h from ${IN}/RefCounter.h.pamphlet
	@${DOCUMENT} src RefCounter.h ${INC}

${DOC}/RefCounter.h.dvi: ${IN}/RefCounter.h.pamphlet ${DOC}/axiom.sty
	@echo 24 making ${DOC}/RefCounter.h.dvi from ${IN}/RefCounter.h.pamphlet
	@${DOCUMENT} dvi RefCounter.h ${DOC} RefCounter.h ${NOISE}


${INC}/Trichotomy.h: ${IN}/Trichotomy.h.pamphlet
	@echo 25 making ${INC}/Trichotomy.h from ${IN}/Trichotomy.h.pamphlet
	@${DOCUMENT} src Trichotomy.h ${INC}

${DOC}/Trichotomy.h.dvi: ${IN}/Trichotomy.h.pamphlet ${DOC}/axiom.sty
	@echo 26 making ${DOC}/Trichotomy.h.dvi from ${IN}/Trichotomy.h.pamphlet
	@${DOCUMENT} dvi Trichotomy.h ${DOC} Trichotomy.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 27 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
