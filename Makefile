FONTE = rasp/src/source/
DFONTE = desktop/src/source/
CFONTE = common/src/source/
FONTES = $(wildcard ${FONTE}*.c)
DFONTES = $(wildcard ${DFONTE}*.c)
CFONTES = $(wildcard ${CFONTE}*.c)
BUILDDIR = build/
PROJECT = ${BUILDDIR}teste

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

RASP_C_OPTIONS = -Irasp/src/include -Icommon/src/include -O2 -g
DESKTOP_C_OPTIONS = -Idesktop/src/include -Icommon/src/include -O2 -g
VALGRIND_OPTIONS = -s --leak-check=full --track-origins=yes --show-leak-kinds=all 

all: compile run 

compile:
	gcc ${RASP_C_OPTIONS} ${CFONTES} ${FONTES} -o ${RTARGET}

tests:
	gcc ${RASP_C_OPTIONS} ${FONTE}math_func.c ${FONTE}matrix.c ${FONTE}main.c -o ${RTARGET}

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
