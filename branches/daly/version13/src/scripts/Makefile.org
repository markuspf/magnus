OUT=${MNT}/${SYS}/bin

all:
	@echo 1 making ${SRC}/scripts
	@cp -pr * ${OUT}
	@rm -f ${OUT}/Makefile*
