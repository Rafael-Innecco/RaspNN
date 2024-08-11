#include "uart.h"

#include <errno.h>  // Error integer and strerror() function
#include <fcntl.h>  // Contains file controls like O_RDWR
#include <stdio.h>
#include <termios.h>  // Contains POSIX terminal control definitions
#include <unistd.h>   // write(), read(), close()

int uart_init(void) {
  // Open the serial port. Change device path as needed (currently set to an
  // standard FTDI USB-UART cable type device)
  int handler = open(UART_PORT_PATH, O_RDWR);

  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if (tcgetattr(handler, &tty) != 0) {
    perror("Error from tcgetattr");
    return -1;
  }

  tty.c_cflag &= ~PARENB;  // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB;  // Clear stop field, only one stop bit used in
                           // communication (most common)
  tty.c_cflag &= ~CSIZE;   // Clear all bits that set the data size
  tty.c_cflag |= CS8;      // 8 bits per byte (most common)
  tty.c_cflag |=
      CREAD | CLOCAL;  // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;    // Disable echo
  tty.c_lflag &= ~ECHOE;   // Disable erasure
  tty.c_lflag &= ~ECHONL;  // Disable new-line echo
  tty.c_lflag &= ~ISIG;    // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);  // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                   ICRNL);  // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST;  // Prevent special interpretation of output bytes
                          // (e.g. newline chars)
  // Prevent conversion of newline to carriage return/line feed
  tty.c_oflag &= ~ONLCR;

  tty.c_cc[VTIME] = 10;  // Wait for up to 1s (10 deciseconds), returning as
                         // soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(handler, TCSANOW, &tty) != 0) {
    perror("Error from tcsetattr");
    return -1;
  }

  return handler;
}

void uart_read(int handler, char* buf, ssize_t size) {
  ssize_t i = 0, num_bytes;
  while (i < size) {
    num_bytes = read(handler, &buf[i], size);
    if (num_bytes > 0) i += num_bytes;
  }
  return;
}

void uart_write(int handler, char* buf, ssize_t size) {
  ssize_t i = 0, num_bytes;
  while (i < size) {
    num_bytes = write(handler, &buf[i], size);
    if (num_bytes > 0) i += num_bytes;
  }
}

void uart_close(int handler) {
  close(handler);
  return;
}
