#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t *read_array(size_t *length) {
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_len = getline(&input, &input_size, stdin);

    if (input_len == -1) {
        fprintf(stderr, "failed to read line: %s.", strerror(errno));
        exit(EXIT_FAILURE);
    }

    *length = 0;
    int32_t *array = (int32_t *)0;
    size_t len = 0;

    char *token = strtok(input, " ");
    while (token != NULL) {
        len++;
        array = realloc(array, sizeof(int32_t) * len);

        if (array == NULL) {
            fprintf(stderr, "failed to allocate memory: %s.", strerror(errno));
            exit(EXIT_FAILURE);
        }

        array[len - 1] = atoi(token);
        token = strtok(NULL, " ");
    }

    free(input);
    *length = len;
    return array;
}

int32_t ascend(const void *a, const void *b) {
    return (*(int32_t*)a - *(int32_t*)b);
}

void display(int32_t array[], size_t length) {
    if (length <= 0) return;

    char buf[BUFSIZ];
    setvbuf(stdout, buf, _IOFBF, BUFSIZ);

    printf("{ ");

    for (size_t i = 0; i < length; i++) {
        printf("%i", array[i]);

        if (i != length - 1) printf(", ");
    }

    printf(" }\n");
    fflush(stdout);
}

int main() {
    size_t len;
    int32_t *array = read_array(&len);

    printf("original: ");
    display(array, len);

    qsort(array, len, sizeof(int32_t), ascend);
    printf("sorted:   ");
    display(array, len);

    free(array);
    return 0;
}
