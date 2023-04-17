#include <sys/utsname.h>
#include <netdb.h>

#include "network.h"

size_t handle_arguments(int argc, char **argv, char **hostname) {
    int option;
    size_t port = 0;

    while ((option = getopt(argc, argv, "p:h:")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                *hostname = optarg;
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

int main(int argc, char **argv) {
    char *hostname = "monomarsh.com";
    size_t port = handle_arguments(argc, argv, &hostname);

    struct utsname uname_data;
    if (uname(&uname_data) == -1) panic("failed to get system data");
    char platform[16];
    sprintf(platform, "%s-%s", uname_data.sysname, uname_data.machine);
    
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL) panic("failed to resolve host");

    int sockfd = open_socket();

    struct sockaddr_in server;
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) panic("failed to connect to server");
    send_line(sockfd, platform);

    close(sockfd);
    return 0;
}
