#include "socket_wrapper.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int socket_connect(char* ip, int port) {
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
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock);
    exit(EXIT_FAILURE);
  }

  // Connect to server
  while (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection failed");
    sleep(1);
  }

  return sock;
}

int socket_listen(int port, int* handler) {
  int new_socket;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // Create socket
  *handler = socket(AF_INET, SOCK_STREAM, 0);
  if (*handler == -1) {
    perror("Socket creation failed");
    return -1;
  }

  // Bind to address and port
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(*handler, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(*handler);
    return -1;
  }

  // Listen for connections
  if (listen(*handler, 3) < 0) {
    perror("Listen failed");
    close(*handler);
    return -1;
  }

  printf("Server listening on port %d...\n", port);

  // Accept a connection
  new_socket =
      accept(*handler, (struct sockaddr*)&address, (socklen_t*)&addrlen);
  if (new_socket < 0) {
    perror("Accept failed");
    close(*handler);
    return -1;
  }

  return new_socket;
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
