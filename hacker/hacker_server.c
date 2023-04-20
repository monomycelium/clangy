#include <arpa/inet.h>
#include <signal.h>
#include <sys/mman.h>

#include "network.h"

uint32_t *counter;
int listener_d;
FILE *file_pointer;

void graceful_shutdown(int sig) {
    if (listener_d) close(listener_d);
    if (file_pointer) fclose(file_pointer);

    printf("\n%i client(s) served.\n", *counter);
    exit(1);
}

void catch_signal(int sig, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1)
        panic("failed to set interrupt handler");
}

size_t handle_arguments(int argc, char **argv, char **file) {
    int option;
    size_t port = 0;

    while ((option = getopt(argc, argv, "p:f:")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'f':
                *file = optarg;
                break;
            case '?':
                fprintf(stderr, "invalid option: %c.\n", optopt);
                exit(1);
            default:
                fprintf(stderr, "unknown option: %c.\n", option);
                exit(1);
        }
    }

    for (int index = optind; index < argc; index++) {
        printf("unknown option: %s\n", argv[index]);
    }
    if (optind < argc) exit(1);

    if (port == 0) port = 6969;
    return port;
}

int main(int argc, char **argv) {
    catch_signal(SIGINT, graceful_shutdown);

    char *file = "hacker.csv";
    size_t port = handle_arguments(argc, argv, &file);

    listener_d = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_d < 0) panic("failed to open socket");
    bind_port(listener_d, port);
    if (listen(listener_d, 10) == -1) panic("failed to listen to port");

    printf("waiting for connection on port %zu...", port);
    fflush(stdout);

    counter = mmap(NULL, sizeof(uint32_t), PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (counter == MAP_FAILED) panic("failed to map memory");
    *counter = 0;

    struct sockaddr_storage client_address;
    socklen_t address_len = sizeof(client_address);
    char buffer[BUFSIZ];

    file_pointer = fopen(file, "a+");
    if (file_pointer == NULL) panic("failed to open file");

    while (1) {
        int connect_d = accept(listener_d, (struct sockaddr *)&client_address,
                               &address_len);
        if (connect_d == -1) panic("failed to open secondary socket");

        pid_t pid = fork();
        if (pid == -1) panic("failed to fork process");

        if (pid != 0) {
            close(connect_d);
            continue;
        }

        close(listener_d);

        *counter += 1;

        struct sockaddr_in *client_address_host_byte =
            (struct sockaddr_in *)&client_address;
        char *client_ip = inet_ntoa(client_address_host_byte->sin_addr);
        if (client_ip == NULL) panic("failed to convert ip address");
        fprintf(file_pointer, "\n%s", client_ip);
        printf("\nc %s", client_ip);
        fflush(stdout);

        ssize_t received_bytes;
        while ((received_bytes = recv(connect_d, buffer, sizeof(buffer), 0)) >
               0) {
            if (fwrite(buffer, 1, received_bytes, file_pointer) !=
                received_bytes)
                panic("failed to write to file");
            memset(buffer, 0, sizeof(buffer));
        }
        if (received_bytes < 0) panic("failed to receive buffer");
        printf("\nm %s", client_ip);
        fflush(stdout);

        printf("\nd %s", client_ip);
        fflush(stdout);
        exit(0);
        close(connect_d);
    }

    fclose(file_pointer);
    close(listener_d);
    return 0;
}
