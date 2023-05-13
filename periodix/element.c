#include "element.h"

#include <strings.h>

#include "commons.h"

const char *const element_names[] = {
    "",          "Hydrogen",  "Helium",  "Lithium",    "Beryllium", "Boron",
    "Carbon",    "Nitrogen",  "Oxygen",  "Fluorine",   "Neon",      "Sodium",
    "Magnesium", "Aluminum",  "Silicon", "Phosphorus", "Sulfur",    "Chlorine",
    "Argon",     "Potassium", "Calcium"};

const char *const element_symbols[] = {"",   "H", "He", "Li", "Be", "B",  "C",
                                       "N",  "O", "F",  "Ne", "Na", "Mg", "Al",
                                       "Si", "P", "S",  "Cl", "Ar", "K",  "Ca"};

const char *get_name(element *atom) {
    return element_names[atom->atomic_number];
}

const char *get_symbol(element *atom) {
    return element_symbols[atom->atomic_number];
}

shells get_configuration(element *atom) {
    shells configuration = {0};

    uint8_t count = atom->atomic_number;
    for (uint8_t i = 0; i < MAX_SHELLS; i++) {
        uint8_t max_electrons = (i == 0) ? 2 : 8;

        configuration.shells[i] =
            (count > max_electrons) ? max_electrons : count;
        configuration.shell_count++;

        if (count <= max_electrons) break;
        count -= configuration.shells[i];
    }

    return configuration;
}

uint8_t get_period(element *atom) { return atom->configuration.shell_count; }

uint8_t get_group(element *atom) {
    if (atom->group > 2)
        return atom->configuration.shells[atom->configuration.shell_count - 1] +
               10;
    else
        return atom->configuration.shells[atom->configuration.shell_count - 1];
}

uint8_t get_number_position(element *atom) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < atom->period - 1; i++) {
        count += (!i) ? 2 : 8;
    }

    count += atom->group % 10;
    return count;
}

uint8_t get_number_name(element *atom) {
    for (uint8_t i = 0; i <= MAX_NUMBER; i++) {
        if (!strcasecmp(element_names[i], atom->name)) return i;
    }

    return 0;
}

uint8_t get_number_symbol(element *atom) {
    for (uint8_t i = 0; i <= MAX_NUMBER; i++) {
        if (!strcasecmp(element_symbols[i], atom->symbol)) return i;
    }

    return 0;
}

uint8_t get_number(element *atom, option option) {
    switch (option) {
        case POSITION:
            return get_number_position(atom);

        case NAME:
            return get_number_name(atom);

        case SYMBOL:
            return get_number_symbol(atom);

        default:
            return 0;
    }
}

bool get_all(element *atom, option option) {
    if (option != NUMBER) atom->atomic_number = get_number(atom, option);
    if (!atom->atomic_number || atom->atomic_number > MAX_NUMBER) return false;

    atom->name = get_name(atom);
    atom->symbol = get_symbol(atom);
    atom->configuration = get_configuration(atom);

    if (option == POSITION) {
        if (get_period(atom) != atom->period || get_group(atom) != atom->group)
            return false;
        else
            return true;
    }

    atom->period = get_period(atom);
    atom->group = get_group(atom);

    return true;
}
