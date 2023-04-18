#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <getopt.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

void panic(char *message);
void bind_port(int fd, size_t port);
int open_socket(void);
void send_line(int fd, char *s);
void send_usize(int fd, size_t s);
size_t read_line(int fd, char *buf, size_t len);
size_t read_usize(int fd);

#endif
