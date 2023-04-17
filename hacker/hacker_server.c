#include <signal.h>
#include <sys/mman.h>
#include <arpa/inet.h>

#include "network.h"

uint32_t *counter;
int listener_d;

void graceful_shutdown(int sig) {
    if (listener_d) close(listener_d);

    printf("\n%i client(s) served.\n", *counter);
    exit(1);
}

void catch_signal(int sig, void(*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) == -1) panic("failed to set interrupt handler");
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
        printf ("unknown option: %s\n", argv[index]);
    }
    if (optind < argc) exit(1);

    if (port == 0) port = 3000;
    return port;
}

void steal_data(char *client_ip, char *client_arch, char *path) {
    FILE *file_pointer;

    file_pointer = fopen(path, "a+");
    fprintf(file_pointer, "%s,%s\n", client_ip, client_arch);

    fclose(file_pointer);
}

int main(int argc, char **argv) {
    catch_signal(SIGINT, graceful_shutdown);

    char *file = "hacker.csv";
    size_t port = handle_arguments(argc, argv, &file);

    listener_d = open_socket();
    bind_port(listener_d, port);
    if (listen(listener_d, 10) == -1) panic("failed to listen to port");

    printf("waiting for connection on port %zu...", port);
    fflush(stdout);

    counter = mmap(NULL, sizeof(uint32_t), PROT_READ | PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1, 0);
    if (counter == MAP_FAILED) panic("failed to map memory");
    *counter = 0;

    struct sockaddr_storage client_address;
    socklen_t address_len = sizeof(client_address);
    char buffer[16];

//     char *advice[] = {
//         "don't bother with idealism.\r\n",
//         "forget jayden.\r\n",
//         "read John Green.\r\n",
//         "STOP USING THIS STUPID APP!\r\n",
//         "hear the ring of hope ringing faintly.\r\n"
//     };

    while (1) {
        int connect_d = accept(listener_d, (struct sockaddr *)&client_address, &address_len);
        if (connect_d == -1) panic("failed to open secondary socket");

        if (!fork()) {
            close(listener_d);

            *counter += 1;

            struct sockaddr_in *client_address_host_byte = (struct sockaddr_in *)&client_address;
            char *client_ip = inet_ntoa(client_address_host_byte->sin_addr);
            if (client_ip == NULL) panic("failed to convert ip address");
            printf("\nc %s", client_ip);
            fflush(stdout);

            read_line(connect_d, buffer, sizeof(buffer));
            printf("\nm %s: %s", client_ip, buffer);

//             char *msg = advice[rand() % 5];
//             send_line(connect_d, msg);

            steal_data(client_ip, buffer, file);

//             while (strncasecmp("q", buffer, 1)) {
//                 printf("\nclient: %s", buffer);
//                 fflush(stdout);
//                 send_line(connect_d, "> ");
//                 read_line(connect_d, buffer, sizeof(buffer));
//             }

            close(connect_d);
            printf("\nd %s", client_ip);
            fflush(stdout);
            exit(0);
        }

        close(connect_d);
    }

    close(listener_d);
    return 0;
}
