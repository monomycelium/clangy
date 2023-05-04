#include "commons.h"

void panic(const char *message) {
    if (errno)
        fprintf(stderr, "%s: %s.\n", message, strerror(errno));
    else
        fprintf(stderr, "%s.\n", message);

    exit(errno);
}
