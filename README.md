# RaspNN

Esse projeto tem como objetivo o desenvolvimento de uma Rede Neural na linguagem C utilizando recursos de hardware específicos do Raspberry Pi para tornar o treinamento e a inferência mais rápida.

O recurso de aceleração escolhido para esse projeto foi o ARM NEON, trata-se de um processador vetorial da ARM capaz de processar 128 bits (na versão v7) paralelamente. Para a utilização desse foi utilizada a biblioteca <arm_neon.h> (também conhecida como ARM Intrinsics) que contêm diversos wrappers para as instruções assembly desse processador vetorial.

Para facilitar o porte desse projeto outros aceleradores, encapsulou-se a lógica do ARM NEON nos arquivos rasp/src/source/matrix.c e rasp/src/source/math_func.c.

A Rede Neural desenvolvida para esse projeto tem apenas duas camadas (entrada e saída, não há camadas escondidas) e tem como objetivo o reconhecimento de dígitos do conjunto de dados do MNIST. Ela pode ser encontrada no arquivo rasp/src/source/neural_network.c.

Além disso, foi criada uma Aplicação Cliente-Servidor (sobre o protocolo TCP) para a comunicação entre o Raspberry Pi e o Computador. Desse modo, um computador qualquer pode enviar imagens e rótulos para a Raspberry Pi para que ela possa realizar o treinamento/inferência da rede.

Esse projeto foi testado em uma Raspberry Pi 3B+, todavia acredita-se que qualquer processador ARM que suporte o ARM NEON v7 será capaz de executar esse projeto sem problemas. Contudo, tenha atenção ao compilador utilizado, pois foi verificado que diversos compiladores cruzados não geravam as instruções de máquina corretas do NEON. Desse modo, aconselha-se a utilizar compiladores nativos, em especial notou-se que o compilador do Raspbian OS Lite gera os binários esperados.

## Instruções de Uso

Para compilar e executar a aplicação servidor do Raspberry Pi quanto a aplicação cliente Desktop basta utilizar os targets corretos do Makefile presente na raíz desse diretório.

Primeiramente, deve-se criar os diretórios build dentro dos diretórios desktop e rasp, como pode ser visto a seguir:

```bash
mkdir rasp/build/
mkdir desktop/build/
```

Para compilar e executar o servidor na Raspberry Pi, basta utilizar o comando:

```bash
make
```

Para compilador o servidor na Raspberry Pi:

```bash
make compile
```

Para testar as funções do matrix e do math_func do servidor na Raspberry Pi:

```bash
make tests
```

Para executar o servidor na Raspberry Pi:

```bash
make run
```

Para executar o servidor na Raspberry Pi com o [Valgrind](https://valgrind.org/), basta utilizar o comando:

```bash
make check
```

Para gerar os arquivos em linguagem de montagem do Raspberry Pi, basta utilizar o comando:

```bash
make assembly
```

Para compilador o cliente no desktop:

```bash
make build_client
```

Para limpar os executáveis criados:

```bash
make build_client
```
