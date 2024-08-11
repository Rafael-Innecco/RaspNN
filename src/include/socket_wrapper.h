

#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <unistd.h>

#define MTU 1024
#define HOST_IP "192.168.0.0"
#define HOST_PORT 8080

int socket_connect(char* ip, int port);

int socket_listen(int port, int* handler);

void socket_read(int sock, char* buf, ssize_t size);

void socket_write(int sock, char* buf, ssize_t size);

void socket_close(int sock);

#endif  // SOCKETWRAPPER_H
