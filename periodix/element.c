#include "element.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const element_names[] = {
    "",          "Hydrogen",  "Helium",  "Lithium",    "Beryllium", "Boron",
    "Carbon",    "Nitrogen",  "Oxygen",  "Fluorine",   "Neon",      "Sodium",
    "Magnesium", "Aluminum",  "Silicon", "Phosphorus", "Sulfur",    "Chlorine",
    "Argon",     "Potassium", "Calcium"};

const char *const element_symbols[] = {"",   "H", "He", "Li", "Be", "B",  "C",
                                       "N",  "O", "F",  "Ne", "Na", "Mg", "Al",
                                       "Si", "P", "S",  "Cl", "Ar", "K",  "Ca"};

void get_name(element *atom) {
    atom->name = element_names[atom->atomic_number];
}

void get_symbol(element *atom) {
    atom->symbol = element_symbols[atom->atomic_number];
}

void get_configuration(element *atom) {
    uint8_t count = atom->atomic_number;
    for (uint8_t i = 0; i < MAX_SHELLS; i++) {
        uint8_t max_electrons = (i == 0) ? 2 : 8;

        atom->configuration.shells[i] =
            (count > max_electrons) ? max_electrons : count;
        atom->configuration.shell_count++;

        if (count <= max_electrons) return;
        count -= atom->configuration.shells[i];
    }
}

void get_period(element *atom) {
    atom->period = atom->configuration.shell_count;
}

void get_group(element *atom) {
    atom->group =
        atom->configuration.shells[atom->configuration.shell_count - 1];
    if (atom->group > 2) atom->group += 10;
}

void panic(const char *message) {
    if (errno)
        fprintf(stderr, "%s: %s.\n", message, strerror(errno));
    else
        fprintf(stderr, "%s.\n", message);

    exit(errno);
}
