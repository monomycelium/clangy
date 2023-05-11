#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_SHELLS 4
#define MAX_NUMBER 20
#define MAX_GROUPS 18
#define MAX_PERIODS 7

extern const char *const element_names[];
extern const char *const element_symbols[];

typedef struct {
    uint8_t shells[MAX_SHELLS];
    uint8_t shell_count : 7;
} shells;

typedef struct {
    uint8_t atomic_number;
    uint8_t period;
    uint8_t group;
    const char *name;
    const char *symbol;
    shells configuration;
} element;

typedef enum { NUMBER, POSITION, NAME, SYMBOL, NONE } option;

const char *get_name(element *atom);               // depends on atomic_number
const char *get_symbol(element *atom);             // depends on atomic_number
shells get_configuration(element *atom);           // depends on atomic_number
uint8_t get_period(element *atom);                 // depends on configuration
uint8_t get_group(element *atom);                  // depends on configuration
uint8_t get_number_name(element *atom);            // depends on name
uint8_t get_number_symbol(element *atom);          // depends on symbol
uint8_t get_number_position(element *atom);        // depends on group, period
uint8_t get_number(element *atom, option option);  // depends on get_number_*()
bool get_all(element *atom, option option);        // depends on atomic_number

#endif
