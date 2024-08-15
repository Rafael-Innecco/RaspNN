#include "socket_wrapper.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int socket_connect(char* ip, int port) {
  int status, sock;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n\033[31m Socket creation error \033[0m\n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
    printf("\n\033[31mInvalid address/ Address not supported \033[0m\n");
    return -1;
  }

  if ((status = connect(sock, (struct sockaddr*)&serv_addr,
                        sizeof(serv_addr))) < 0) {
    printf("\n\033[31mConnection Failed \033[0m\n");
    return -1;
  }

  return sock;
}

int socket_server_init(int port, struct sockaddr_in* address) {
  int new_socket, handler;
  int opt = 1;
  socklen_t addrlen = sizeof((*address));

  if ((handler = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("\033[31mSocket creation failed\033[0m");
    return -1;
  }

  if (setsockopt(handler, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("\033[31msetsockopt\033[0m");
    return -1;
  }

  (*address).sin_family = AF_INET;
  (*address).sin_addr.s_addr = INADDR_ANY;
  (*address).sin_port = htons(port);

  if (bind(handler, (struct sockaddr*)address, sizeof((*address))) < 0) {
    perror("\033[31mBind failed\033[0m");
    return -1;
  }

  if (listen(handler, 1) < 0) {
    perror("\033[31mListen failed\033[0m");
    return -1;
  }

  return handler;
}

int socket_listen(int port, int handler, struct sockaddr_in* address) {
  int new_socket = -1;
  socklen_t addrlen = sizeof((*address));
  if ((new_socket = accept(handler, (struct sockaddr*)address, &addrlen)) < 0) {
    perror("\033[31mAccept failed\033[0m");
    return -1;
  }
  return new_socket;
}

void socket_read(int sock, uint8_t* buf, ssize_t size) {
  ssize_t i = 0, num_bytes = (size > MTU) ? MTU : size;
  while (i < size) {
    num_bytes = read(sock, &buf[i], num_bytes);
    if (num_bytes > 0) i += num_bytes;
    num_bytes = ((size - i) > MTU) ? MTU : (size - i);
  }
  return;
}

void socket_write(int sock, uint8_t* buf, ssize_t size) {
  ssize_t i = 0, num_bytes = (size > MTU) ? MTU : size;
  while (i < size) {
    num_bytes = send(sock, &buf[i], num_bytes, 0);
    if (num_bytes >= 0) i += num_bytes;
    num_bytes = ((size - i) > MTU) ? MTU : (size - i);
  }
}

void socket_close(int sock) {
  close(sock);
  return;
}
