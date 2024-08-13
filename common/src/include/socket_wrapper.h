#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <arpa/inet.h>
#include <unistd.h>

#define MTU 1024

int socket_connect(char* ip, int port);

int socket_server_init(int port, struct sockaddr_in* address);

int socket_listen(int port, int handler, struct sockaddr_in* address);

void socket_read(int sock, char* buf, ssize_t size);

void socket_write(int sock, char* buf, ssize_t size);

void socket_close(int sock);

#endif  // SOCKETWRAPPER_H
