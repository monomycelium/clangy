#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SYMBOL "add"
typedef int32_t (*function_type)(int32_t, int32_t);

void err(void) { fprintf(stderr, "error: dl: %s", dlerror()); }

int main(int argc, char **argv) {
    void *handle;
    void *func;
    const char *filename;
    int result;
    int x, y, z;

    if (argc != 4) {
        fprintf(stderr, "usage: %s <SHARED_OBJECT> <X> <Y>\n", argv[0]);
        return EXIT_FAILURE;
    }

    x = atoi(argv[2]);
    y = atoi(argv[3]);
    result = EXIT_FAILURE;
    filename = argv[1];

    handle = dlopen(filename, RTLD_LAZY);
    if (handle == NULL) goto panic;

    func = dlsym(handle, SYMBOL);
    if (dlerror() != NULL) goto error;

    function_type add = (function_type)func;
    z = add(x, y);
    printf(SYMBOL "(%" PRIi32 ", %" PRIi32 ") = %" PRIi32 "\n", x, y, z);
    result = EXIT_SUCCESS;

close:
    if (dlclose(handle) != 0) goto panic;
    return result;

error:
    err();
    result = EXIT_FAILURE;
    goto close;

panic:
    err();
    return EXIT_FAILURE;
}
