

#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#include <unistd.h>

#define MTU 1024
#define PORT 8080

int socket_init(void);

void socket_read(int sock, char* buf, ssize_t size);

void socket_write(int sock, char* buf, ssize_t size);

void socket_close(int sock);

#endif  // SOCKETWRAPPER_H
