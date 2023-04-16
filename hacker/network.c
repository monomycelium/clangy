#include <stddef.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include "network.h"

void panic(char *message) {
    fprintf(stderr, "%s: %s.\n", message, strerror(errno));
    exit(errno);
}

void bind_port(int fd, size_t port) {
    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1) panic("failed to set reuse option on socket");

    if (bind(fd, (struct sockaddr *)&name, sizeof(name)) == -1) panic("failed to bind port");
}

int open_socket(void) {
    int listener_d = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_d == -1) panic("failed to open socket");

    return listener_d;
}

void send_line(int fd, char *s) {
    if (send(fd, s, strlen(s), 0) == -1) panic("failed to send line");
}

size_t read_line(int fd, char *buf, size_t len) {
    ssize_t bytes_received = recv(fd, buf, len, 0);
    if (bytes_received == -1) panic("failed to receive buffer");

    return bytes_received;
}
