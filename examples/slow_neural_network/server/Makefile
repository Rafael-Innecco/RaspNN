.PHONY: all

all:
	gcc -O2 -I. -c -o socket_wrapper.o socket_wrapper.c
	gcc -O2 -I. -c -o matrix.o matrix.c
	gcc -O2 -I. -c -o neural_network.o neural_network.c
	gcc -O2 -I. -c -o server_fsm.o server_fsm.c
	gcc -O2 -I. -c -o main.o main.c
	gcc socket_wrapper.o server_fsm.o main.o matrix.o neural_network.o -o main -lm

run: all
	clear
	./main

clean:
	rm *.o main
