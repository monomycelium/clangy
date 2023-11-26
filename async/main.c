#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "async.h"

void panic(const char *message, ...) __attribute__((noreturn));

int main(int argc, char **argv) {
    char *pathname;   /**< input path */
    int fd;           /**< file descriptor */
    off_t off;        /**< file offset */
    size_t len;       /**< file size */
    char *buf;        /**< input buffer */
    struct aiocb aio; /**< async handle */
    size_t counter;   /**< idle counter */

    if (argc != 2) panic("usage: %s <FILENAME>", argv[0]);

    pathname = argv[1];
    fd = open(pathname, O_RDONLY);
    if (fd == -1) panic("failed to open file `%s'", pathname);

    // get file size and seek back
    for (uint8_t i = 0; i < 2; i++) {
        off = lseek(fd, 0, i == 0 ? SEEK_END : SEEK_SET);
        if (off == -1) panic("failed to seek file `%s'", pathname);
        if (i == 0) len = (size_t)off;
    }

    buf = malloc(len + 1);
    if (buf == NULL) panic("failed to allocate memory");

    aio = async_init(fd, buf, len, 0);
    if (!async_read(&aio)) panic("failed to start reading file `%s'", pathname);

    counter = 0;
    while (!async_done(&aio))  // .await
        counter++;             // for demo (you could be more productive.)

    if (async_okay(&aio) == -1) panic("failed to read file `%s'", pathname);
    buf[aio.aio_nbytes] = '\0';
    puts(buf);

    printf("counted %zu times while waiting.\n", counter);
    if (close(fd) == -1) panic("failed to close file `%s'", pathname);
    return EXIT_SUCCESS;
}

void panic(const char *message, ...) {
    va_list args;
    va_start(args, message);

    vfprintf(stderr, message, args);
    if (errno)
        fprintf(stderr, ": %s.\n", strerror(errno));
    else
        fprintf(stderr, ".\n");

    va_end(args);
    exit(EXIT_FAILURE);
}
