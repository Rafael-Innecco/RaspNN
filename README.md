# RaspNN

Esse projeto tem como objetivo o desenvolvimento de uma Rede Neural na linguagem C utilizando recursos de hardware específicos do Raspberry Pi para tornar o treinamento e a inferência mais rápida.

O recurso de aceleração escolhido para esse projeto foi o ARM NEON, trata-se de um processador vetorial da ARM capaz de processar 128 bits (na versão v7) paralelamente. Para a utilização desse foi utilizada a biblioteca <arm_neon.h> (também conhecida como ARM Intrinsics) que contêm diversos wrappers para as instruções assembly desse processador vetorial.

Para facilitar o porte desse projeto para outros aceleradores, encapsulou-se a lógica do ARM NEON nos arquivos rasp/src/source/matrix.c e rasp/src/source/math_func.c.

A Rede Neural desenvolvida para esse projeto tem apenas duas camadas (entrada e saída, não há camadas escondidas) e tem como objetivo o reconhecimento de dígitos do conjunto de dados do MNIST. Sua lógica está contida no arquivo rasp/src/source/neural_network.c.

Além disso, foi criada uma Aplicação Cliente-Servidor (sobre o protocolo TCP) para a comunicação entre o Raspberry Pi e o Computador. Desse modo, um computador qualquer pode enviar imagens e rótulos para a placa Raspberry Pi para que ela possa realizar o treinamento/inferência da rede.

Esse projeto foi testado em um Raspberry Pi 3B+, todavia acredita-se que qualquer processador ARM que suporte o ARM NEON v7 será capaz de executar esse projeto sem problemas. Contudo, tenha atenção ao compilador utilizado, pois foi verificado que diversos compiladores cruzados não geravam as instruções de máquina corretas do NEON. Desse modo, aconselha-se a utilizar compiladores nativos, em especial notou-se que o compilador do Raspbian OS Lite gera os binários esperados.

## Instruções de Uso

Para compilar e executar a aplicação servidor do Raspberry Pi quanto a aplicação cliente Desktop basta utilizar os targets corretos do Makefile presente na raíz desse diretório.

Primeiramente, deve-se criar os diretórios build dentro dos diretórios desktop e rasp, como pode ser visto a seguir:

```bash
mkdir rasp/build/
mkdir desktop/build/
```

Para compilar e executar o servidor no Raspberry Pi, basta utilizar o comando:

```bash
make
```

Para compilador o servidor no Raspberry Pi:

```bash
make compile
```

Para testar as funções do matrix e do math_func no Raspberry Pi:

```bash
make tests
```

Para executar o servidor no Raspberry Pi:

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
make clean
```

## Instruções para Execução da Rede Neural Comparativa

Na pasta examples/slow_neural_network, está disponível uma versão simplificada do conjunto cliente-servidor, que pode ser executada em computadores comuns para efeito de comparação de desempenho (um Raspberry Pi 3B+ levou apenas 3.7s para rodar a versão otimizada!).

Para isso, deve-se primeiro acessar a pasta examples/slow_neural_network/server e executar o comando para acionar a aplicação do servidor no terminal.

```bash
make run
```

Em seguida, utilizando outro terminal, deve-se acessar a pasta examples/slow_neural_network/client e executar o mesmo comando

```bash
make run
```
Com isso, o treinamento será automaticamente iniciado, sem a necessidade de nenhuma ação adicional, e em alguns instantes o terminal em que a execução do servidor foi iniciada irá exibir o tempo de execução do treinamento. Após o término do treinamento, ambos os programas serão encerrados automaticamente.
