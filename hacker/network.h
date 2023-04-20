#ifndef NETWORK_H
#define NETWORK_H

#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void panic(char *message);
void bind_port(int fd, size_t port);
void send_usize(int fd, size_t s);
size_t read_line(int fd, char *buf, size_t len);
size_t read_usize(int fd);

#endif
