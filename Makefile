FONTE = rasp/src/source/
FONTES = $(wildcard ${FONTE}*.c)
LDSCRIPT = linker.ld
BUILDDIR = build/
PROJECT = ${BUILDDIR}teste

#
# Arquivos de saída
#
EXEC = ${PROJECT}.elf
MAP = ${PROJECT}.map
IMAGE = ${PROJECT}.img
HEXFILE = ${PROJECT}.hex
LIST = ${PROJECT}.list

AS = as
LD = ld
GCC = gcc
OBJCPY = objcopy
OBJDMP = objdump

ASM_OPTIONS = -g
C_OPTIONS = -Irasp/src/include -mtune=cortex-a53 -O2
LD_OPTIONS = -lc -L/usr/lib/aarch64-linux-gnu

OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

all: ${EXEC} ${IMAGE} ${LIST} ${HEXFILE}

compile:
	gcc -Irasp/src/include -Icommon/src/include -O2 -g  ${FONTE}math_func.c ${FONTE}matrix.c  ${FONTE}main.c -o test.out

run: compile
	./test.out

check:
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all ./test.out

# Criar diretório build
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

#
# Gerar executável
#
${EXEC}: ${OBJETOS} $(BUILDDIR)
	${LD} -T ${LDSCRIPT} -M=${MAP} -o $@  ${OBJETOS} ${LD_OPTIONS}

#
# Gerar imagem
#
${IMAGE}: ${EXEC}
	${OBJCPY} ${EXEC} -O binary ${IMAGE}

#
# Gerar intel Hex
#
${HEXFILE}: ${EXEC}
	${OBJCPY} ${EXEC} -O ihex ${HEXFILE}

#
# Gerar listagem
#
${LIST}: ${EXEC}
	${OBJDMP} -sdt ${EXEC} > ${LIST}

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${C_OPTIONS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} ${ASM_OPTIONS} -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f ${FONTE}*.o ${EXEC} ${MAP} ${LIST} ${IMAGE}
