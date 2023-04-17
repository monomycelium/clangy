#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

void panic(char *message);
void bind_port(int fd, size_t port);
int open_socket(void);
void send_line(int fd, char *s);
size_t read_line(int fd, char *buf, size_t len);

#endif
