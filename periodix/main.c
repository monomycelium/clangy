#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "commons.h"
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
    char buffer[BUFSIZ] = {0};
    setvbuf(stdout, buffer, _IOFBF, BUFSIZ);

    element atom = {0};
    option option = process_arguments(argc, argv, &atom);
    switch (option) {
        case POSITION:
            if (atom.group == 0 || atom.period == 0)
                panic("position should be passed as `<group>,<period>`");
            if (atom.group > MAX_GROUPS)
                panic("group %u not found", atom.group);
            if (atom.period > MAX_PERIODS)
                panic("period %u not found", atom.period);
            break;
        case NONE:
            puts(periodic_table);
            return EXIT_SUCCESS;
        default:
            break;
    }

    if (!get_all(&atom, option)) {
        switch (option) {
            case NUMBER:
                panic("atom with atomic number %u not found",
                      atom.atomic_number);
            case POSITION:
                panic("atom from group %u, period %u not found", atom.group,
                      atom.period);
            case NAME:
                panic("atom with name \"%s\" not found", atom.name);
            case SYMBOL:
                panic("atom with symbol \"%s\" not found", atom.symbol);
            default:
                break;
        }
    }

    display(&atom);
    fflush(stdout);
    return EXIT_SUCCESS;
}
