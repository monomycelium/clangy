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
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) ==
        -1)
        panic("failed to set reuse option on socket");

    if (bind(fd, (struct sockaddr *)&name, sizeof(name)) == -1)
        panic("failed to bind port");
}

void send_usize(int fd, size_t s) {
    uint64_t network_s = (uint64_t)htonl(s);
    if (send(fd, &network_s, sizeof(network_s), 0) == -1)
        panic("failed to send size_t");
}

size_t read_line(int fd, char *buf, size_t len) {
    ssize_t bytes_received = recv(fd, buf, len, 0);
    if (bytes_received < 0) panic("failed to receive buffer");

    buf[strcspn(buf, "\r\n")] = '\0';
    return bytes_received;
}

size_t read_usize(int fd) {
    uint64_t s;
    if (recv(fd, &s, sizeof(uint64_t), 0) == -1)
        panic("failed to receive usize");

    return (size_t)ntohl(s);
}
