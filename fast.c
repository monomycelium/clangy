/**
 * fast
 *
 * a demonstration of mapping files to memory.
 * concepts: virtual memory, error-handling
 *
 * TODO: demonstrate writing capabilities.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    int err;
    void *data;
    size_t len;
} read_result;

read_result read_file(const char *pathname);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *pathname = argv[1];
    read_result r = read_file(pathname);

    if (r.err) {
        fprintf(stderr, "failed to read file `%s`: %s.\n", pathname,
                strerror(r.err));
        return EXIT_FAILURE;
    }

    puts(r.data);
    munmap(r.data, r.len);
    return EXIT_SUCCESS;
}

read_result read_file(const char *pathname) {
    read_result r = {0};

    int fd = open(pathname, O_RDONLY, 0);
    if (fd < 0) return (read_result){.err = errno};

    struct stat info;
    if (fstat(fd, &info) != 0)
        return (read_result){.err = errno};  // TODO: just use lseek(2)
    r.len = (size_t)info.st_size;

    void *data = mmap(NULL, r.len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) return (read_result){.err = errno};
    r.data = data;

    close(fd);
    return r;
}
