#include "commons.h"

void panic(const char *message) {
    if (errno)
        fprintf(stderr, "%s.\n", message);
    else
        fprintf(stderr, "%s: %s.\n", message, strerror(errno));

    exit(errno);
}
