#include <stdio.h>
#include <stdlib.h>

#include "element.h"

void display(element *atom) {
    printf("element name:\t%s\n", atom->name);
    printf("element symbol:\t%s\n", atom->symbol);
    printf("shell count:\t%u\n", atom->configuration.shell_count);
    printf("arrangement:\t");
    for (uint8_t i = 0; i < atom->configuration.shell_count; i++) {
        printf("%u%c", atom->configuration.shells[i],
               (atom->configuration.shell_count - 1 > i) ? ',' : ' ');
    }
    putchar('\n');

    printf("period:\t\t%u\n", atom->period);
    printf("group:\t\t%u\n", atom->group);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <ATOMIC_NUMBER>.\n", argv[0]);
        return EXIT_FAILURE;
    }

    char buffer[256] = {0};
    setvbuf(stdout, buffer, _IOFBF, 256);

    element atom = {.atomic_number = atoi(argv[1]), 0};
    if (atom.atomic_number > MAX_NUMBER || atom.atomic_number == 0) {
        fprintf(stderr, "atom %u not found.\n", atom.atomic_number);
        return EXIT_FAILURE;
    }

    get_name(&atom);
    get_symbol(&atom);
    get_configuration(&atom);
    get_period(&atom);
    get_group(&atom);

    display(&atom);
    fflush(stdout);
    return EXIT_SUCCESS;
}
