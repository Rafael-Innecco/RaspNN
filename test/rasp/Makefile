.PHONY: all

all:
	gcc -g -I. -c -o socket_wrapper.o socket_wrapper.c
	gcc -g -I. -c -o matrix.o matrix.c
	gcc -g -I. -c -o neural_network.o neural_network.c
	gcc -g -I. -c -o server_fsm.o server_fsm.c
	gcc -g -I. -c -o main.o main.c
	gcc socket_wrapper.o server_fsm.o main.o matrix.o neural_network.o -o main -lm

run: all
	clear
	./main