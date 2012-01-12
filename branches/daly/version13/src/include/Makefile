IN=${SRC}/include
INC=${INT}/include
DOC=${MNT}/${SYS}/doc/src/include
TEST=${OBJ}/${SYS}/test

FILES= ${INC}/AllocRing.h ${INC}/builtin.h  ${INC}/Integer.h \
       ${INC}/Integer.hP  ${INC}/Rational.h ${INC}/std.h \
       ${INC}/String.h

DOCFILES= ${DOC}/AllocRing.h.dvi ${DOC}/builtin.h.dvi  ${DOC}/Integer.h.dvi \
          ${DOC}/Integer.hP.dvi  ${DOC}/Rational.h.dvi ${DOC}/std.h.dvi \
          ${DOC}/String.h.dvi

TESTCASES=${TEST}/testcase

all: ${FILES} ${DOCFILES} ${TESTCASES}
	@echo 14 finished ${IN}

${INC}/AllocRing.h: ${IN}/AllocRing.h.pamphlet
	@echo 1 making ${INC}/AllocRing.h from ${IN}/AllocRing.h.pamphlet
	@${DOCUMENT} src AllocRing.h ${INC} 

${DOC}/AllocRing.h.dvi: ${IN}/AllocRing.h.pamphlet ${DOC}/axiom.sty
	@echo 2 making ${DOC}/AllocRing.h.dvi from ${IN}/AllocRing.h.pamphlet
	@${DOCUMENT} dvi AllocRing.h ${DOC} AllocRing.h ${NOISE}

${TEST}/testcase: ${IN}/AllocRing.h.pamphlet
	@echo 3 making ${TEST}/testcase from ${IN}/AllocRing.h.pamphlet
	@${DOCUMENT} chunk testcase AllocRing.h ${TEST} testcase.c
	@g++ ${CFLAGS} ${INCLUDE} -o ${TEST}/testcase ${TEST}/testcase.c
	@${TEST}/testcase


${INC}/builtin.h: ${IN}/builtin.h.pamphlet
	@echo 4 making ${INC}/builtin.h from ${IN}/builtin.h.pamphlet
	@${DOCUMENT} src builtin.h ${INC}

${DOC}/builtin.h.dvi: ${IN}/builtin.h.pamphlet ${DOC}/axiom.sty
	@echo 5 making ${DOC}/builtin.h.dvi from ${IN}/builtin.h.pamphlet
	@${DOCUMENT} dvi builtin.h ${DOC} builtin.h ${NOISE}


${INC}/Integer.h: ${IN}/Integer.h.pamphlet
	@echo 6 making ${INC}/Integer.h from ${IN}/Integer.h.pamphlet
	@${DOCUMENT} src Integer.h ${INC}

${DOC}/Integer.h.dvi: ${IN}/Integer.h.pamphlet ${DOC}/axiom.sty
	@echo 7 making ${DOC}/Integer.h.dvi from ${IN}/Integer.h.pamphlet
	@${DOCUMENT} dvi Integer.h ${DOC} Integer.h ${NOISE}


${INC}/Integer.hP: ${IN}/Integer.hP.pamphlet
	@echo 8 making ${INC}/Integer.hP from ${IN}/Integer.hP.pamphlet
	@${DOCUMENT} src Integer.hP ${INC}

${DOC}/Integer.hP.dvi: ${IN}/Integer.hP.pamphlet ${DOC}/axiom.sty
	@echo 9 making ${DOC}/Integer.hP.dvi from ${IN}/Integer.hP.pamphlet
	@${DOCUMENT} dvi Integer.hP ${DOC} Integer.hP ${NOISE}


${INC}/Rational.h: ${IN}/Rational.h.pamphlet
	@echo 10 making ${INC}/Rational.h from ${IN}/Rational.h.pamphlet
	@${DOCUMENT} src Rational.h ${INC}

${DOC}/Rational.h.dvi: ${IN}/Rational.h.pamphlet ${DOC}/axiom.sty
	@echo 11 making ${DOC}/Rational.h.dvi from ${IN}/Rational.h.pamphlet
	@${DOCUMENT} dvi Rational.h ${DOC} Rational.h ${NOISE}


${INC}/std.h: ${IN}/std.h.pamphlet
	@echo 12 making ${INC}/std.h  from ${IN}/std.h.pamphlet
	@${DOCUMENT} src std.h ${INC} 

${DOC}/std.h.dvi: ${IN}/std.h.pamphlet ${DOC}/axiom.sty
	@echo 13 making ${DOC}/std.h.dvi from ${IN}/std.h.pamphlet
	@${DOCUMENT} dvi std.h ${DOC} std.h ${NOISE}


${INC}/String.h: ${IN}/String.h.pamphlet
	@echo 14 making ${INC}/String.h from ${IN}/String.h.pamphlet
	@${DOCUMENT} src String.h ${INC} 

${DOC}/String.h.dvi: ${IN}/String.h.pamphlet ${DOC}/axiom.sty
	@echo 15 making ${DOC}/String.h.dvi from ${IN}/String.h.pamphlet
	@${DOCUMENT} dvi String.h ${DOC} String.h ${NOISE}


${DOC}/axiom.sty: ${SRC}/doc/axiom.sty.pamphlet
	@ echo 16 making ${DOC}/axiom.sty from ${SRC}/doc/axiom.sty.pamphlet
	@${DOCUMENT} chunk "axiom.sty" ${SRC}/doc/axiom.sty ${DOC} axiom.sty
 
