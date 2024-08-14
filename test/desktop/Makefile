.PHONY: all

all:
	gcc -g -I. -c -o socket_wrapper.o socket_wrapper.c
	gcc -g -I. -c -o mnist_file.o mnist_file.c
	gcc -g -I. -c -o client_cli.o client_cli.c
	gcc client_cli.o socket_wrapper.o mnist_file.o -o client_cli