FONTE = rasp/src/source/
DFONTE = desktop/src/source/
CFONTE = common/src/source/
FONTES = $(wildcard ${FONTE}*.c)
DFONTES = $(wildcard ${DFONTE}*.c)
CFONTES = $(wildcard ${CFONTE}*.c)
BUILDDIR = build/
PROJECT = ${BUILDDIR}teste

TESTSOURCE = examples/main_test.c
TESTTARGET = examples/build/

#
# Arquivos de saída
#
RTARGET = rasp/${BUILDDIR}rasp.out
DTARGET = desktop/${BUILDDIR}desk.out

AS = as
LD = ld
GCC = gcc
OBJCPY = objcopy
OBJDMP = objdump

RASP_C_OPTIONS = -Irasp/src/include -Icommon/src/include -O2
DESKTOP_C_OPTIONS = -Idesktop/src/include -Icommon/src/include -O2
VALGRIND_OPTIONS = -s --leak-check=full --track-origins=yes --show-leak-kinds=all 

all: compile run 

compile:
	gcc ${RASP_C_OPTIONS} ${CFONTES} ${FONTES} -o ${RTARGET} -lm

tests:
	gcc -g -Irasp/src/include -c -o ${TESTTARGET}math_func.o ${FONTE}math_func.c
	gcc -g -Irasp/src/include -c -o ${TESTTARGET}matrix.o ${FONTE}matrix.c
	gcc -g -Irasp/src/include -c -o ${TESTTARGET}main.o ${TESTSOURCE} 
	gcc ${TESTTARGET}math_func.o ${TESTTARGET}matrix.o ${TESTTARGET}main.o -o ${TESTTARGET}test -lm 
	${TESTTARGET}test

run: ${RTARGET}
	${RTARGET}

check:
	valgrind ${VALGRIND_OPTIONS} ${RTARGET}

assmeble:
	gcc -Irasp/src/include -Icommon/src/include -O2 -S ${FONTE}math_func.c -o rasp/${BUILDDIR}/math_func.s
	gcc -Irasp/src/include -Icommon/src/include -O2 -S ${FONTE}matrix.c -o rasp/${BUILDDIR}/matrix.s
	gcc -Irasp/src/include -Icommon/src/include -O2 -S ${FONTE}main.c -o rasp/${BUILDDIR}/main.s

build_client:
	gcc ${DESKTOP_C_OPTIONS} ${CFONTES} ${DFONTES} -o ${DTARGET}
	
#
# Limpar tudo
#
clean:
	rm -f ${RTARGET} ${DTARGET}
