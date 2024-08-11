

#ifndef UART_H
#define UART_H

#include <unistd.h>

#define UART_PORT_PATH "/dev/ttyUSB0"

int uart_init(void);

void uart_read(int handler, char* buf, ssize_t size);

void uart_write(int handler, char* buf, ssize_t size);

void uart_close(int handler);

#endif  // UART_H
