#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "jpegdump.h"

typedef struct {
    int err;
    void *data;
    size_t len;
} read_result;

void panic(const char *message, ...) __attribute__((noreturn));
read_result read_file(const char *pathname);

int main(int argc, char **argv) {
    if (argc != 2) panic("usage: %s <FILENAME>", argv[0]);
    const char *pathname = argv[1];

    read_result file = read_file(pathname);
    if (file.err) panic("failed to read file `%s'", pathname);

    jpeg_t result = jpegdump((uint8_t *)file.data, file.len);
    if (result.err != JD_SUCCESS) {
        fprintf(
            stderr, "%s: `%s'.\n",
            result.err == JD_MISSING_INFO ? "missing metadata" : "invalid JPEG",
            pathname);
        return EXIT_FAILURE;
    }

    printf("image size: %hu × %hu pixels\n", result.width, result.height);
    munmap(file.data, file.len);
}

// memory-mapped file reader
read_result read_file(const char *pathname) {
    read_result r = {0};

    int fd = open(pathname, O_RDONLY, 0);
    if (fd < 0) return (read_result){.err = errno};

    struct stat info;
    if (fstat(fd, &info) != 0) return (read_result){.err = errno};
    r.len = (size_t)info.st_size;

    void *data = mmap(NULL, r.len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) return (read_result){.err = errno};
    r.data = data;

    close(fd);
    return r;
}

// die function
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
