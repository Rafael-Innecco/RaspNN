FONTE = src/source/
FONTES = ${FONTE}boot.s $(wildcard ${FONTE}*.c)
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

PREFIXO = arm-linux-gnueabihf-
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJCPY = ${PREFIXO}objcopy
OBJDMP = ${PREFIXO}objdump

ASM_OPTIONS = -g
C_OPTIONS = -march=armv7-a -mfpu=neon-vfpv4 -mtune=cortex-a7 -mfloat-abi=hard -g -Isrc/include
LD_OPTIONS = -lc -L/usr/lib/arm-none-eabi/newlib

OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

all: ${EXEC} ${IMAGE} ${LIST} ${HEXFILE}

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
