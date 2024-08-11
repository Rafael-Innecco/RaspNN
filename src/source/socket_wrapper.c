#include "socket_wrapper.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int socket_init(void) {
  int sock;
  struct sockaddr_in serv_addr;

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket creation failed");
  }

  // Specify server address
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Connect to server
  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection failed");
    close(sock);
  }

  return sock;
}

void socket_read(int sock, char* buf, ssize_t size) {
  ssize_t i = 0, num_bytes;
  while (i < size) {
    num_bytes = recv(sock, &buf[i], (i % MTU), 0);
    if (num_bytes > 0) i += num_bytes;
  }
  return;
}

void socket_write(int sock, char* buf, ssize_t size) {
  ssize_t i = 0, num_bytes;
  while (i < size) {
    num_bytes = send(sock, &buf[i], (i % MTU), 0);
    if (num_bytes > 0) i += num_bytes;
  }
}

void socket_close(int sock) {
  close(sock);
  return;
}
