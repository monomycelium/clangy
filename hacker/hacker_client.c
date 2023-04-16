#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "network.h"

int main(void) {
    struct utsname uname_data;
    if (uname(&uname_data) == -1) panic("failed to get system data");
    char platform[16];
    sprintf(platform, "%s-%s", uname_data.sysname, uname_data.machine);
    
    struct hostent *host = gethostbyname("monomarsh.com");
    if (host == NULL) panic("failed to resolve host");

    int sockfd = open_socket();

    struct sockaddr_in server;
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(6969);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) panic("failed to connect to server");
    send_line(sockfd, platform);

    close(sockfd);
    return 0;
}
