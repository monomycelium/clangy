#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "totp.h"

typedef struct {
    opts_t opts; /**< options */
    uint8_t *k; /**< key */
} app_t;

void stou(const char *ptr, time_t *val);
bool parseargs(int argc, char **argv, app_t *app);
void usage(int code, char *name);
uint8_t *readntrim(FILE *restrict stream);
void usage(int code, char *name) {
    fprintf(stderr, "usage: %s [OPTIONS]\n", name);
    fputs(
        "  -t <TIME: int>\tUnix time to start time steps (default: 0)\n"
        "  -k <KEY: str>\tsecret key to solve (default: read from stdin)\n"
        "  -x <STEPS: uint>\ttime steps in seconds (default: 30)\n",
        stderr);
    exit(code);
}

int main(int argc, char **argv) {
    time_t t;      // time
    uint32_t res;  // result
    char *err;     // error
    app_t app; /**< application options */
    bool read; /**< whether the key was read from stdin */

    app.opts.x = 30;
    app.opts.t0 = 0;
    app.k = NULL;

    if (parseargs(argc, argv, &app) == false)
        usage(EXIT_FAILURE, argv[0]);

    if ((read = app.k == NULL)) {
        app.k = readntrim(stdin);
        if (app.k == NULL) {
            err = "failed to read secret";
            goto erron;
        }
    }

    t = time(NULL);
    if (t == (time_t)-1) {
        err = "time overflow!\n";
        goto error;
    }
    t = (t - app.opts.t0) / app.opts.x;

    res = totp_calc(app.k, t);
    switch (res) {
        case (uint32_t)-1:
            err = "invalid secret\n";
            goto error;
        case (uint32_t)-2:
            err = "HMAC(3) failed\n";
            goto error;
        default:
            break;
    }

    printf("%06u\n", res);
    if (read) free(app.k);
    return EXIT_SUCCESS;

error:
    fputs(err, stderr);
    if (read && app.k != NULL) free(app.k);
    return EXIT_FAILURE;

erron:
    fprintf(stderr, "%s: %s\n", err, strerror(errno));
    return EXIT_FAILURE;
}

void stou(const char *ptr, time_t *val) {
    char *endptr;
    long result;

    endptr = NULL;
    result = strtol(ptr, &endptr, 10);
    if (*endptr != '\0' && errno != ERANGE) *val = result;
}

bool parseargs(int argc, char **argv, app_t *app) {
    int c;

    while ((c = getopt(argc, argv, "t:x:k:")) != -1) switch (c) {
        case 't':
            stou(optarg, &(app->opts.t0));
            break;
        case 'x':
            stou(optarg, &(app->opts.x));
            break;
        case 'k':
            app->k = (uint8_t *)optarg;
            break;
        case '?':
        default:
            return false;
    }

    return true;
}

uint8_t *readntrim(FILE *restrict stream) {
    char *line; /**< line pointer */
    size_t bufsiz; /**< buffer size */
    ssize_t buflen; /**< buffer length */

    bufsiz = 0;
    line = NULL;

    buflen = getline(&line, &bufsiz, stream);
    if (buflen == -1) return NULL;

    if (line[buflen - 1] == '\n') line[buflen - 1] = '\0';
    return (uint8_t *)line;
}

