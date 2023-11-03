#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "totp.h"

void stou(const char *ptr, size_t *val);

int main(int argc, char **argv) {
    time_t t;      // time
    uint8_t *k;    // key
    opts_t opts;   // options
    uint32_t res;  // result
    char *err;     // error

    opts.x = 30;
    opts.t0 = 0;

    switch (argc) {
        case 4:
            stou(argv[3], (size_t *)&(opts.t0));
        case 3:
            stou(argv[2], (size_t *)&(opts.x));
        case 2:
            k = (uint8_t *)argv[1];
            break;
        default:
            err = "usage: totp <SECRET> [INTERVAL [START]]\n";
            goto error;
    }

    t = time(NULL);
    if (t == (time_t)-1) {
        err = "time overflow!\n";
        goto error;
    }
    t = (t - opts.t0) / opts.x;

    res = totp_calc(k, t);
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
    return EXIT_SUCCESS;

error:
    fputs(err, stderr);
    return EXIT_FAILURE;
}

void stou(const char *ptr, size_t *val) {
    char *endptr;
    size_t result;

    endptr = NULL;
    result = strtoll(ptr, &endptr, 10);
    if (*endptr != '\0' && errno != ERANGE) *val = result;
}
