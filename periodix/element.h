#ifndef ELEMENT_H
#define ELEMENT_H

#include <stdint.h>

#define MAX_SHELLS 4
#define MAX_NUMBER 20

extern const char *const element_names[];
extern const char *const element_symbols[];

typedef struct {
    uint8_t shells[MAX_SHELLS];
    uint8_t shell_count;
} shells;

typedef struct {
    uint8_t atomic_number;
    uint8_t period;
    uint8_t group;
    const char *name;
    const char *symbol;
    shells configuration;
} element;

void get_name(element *atom);           // depends on atomic_number
void get_symbol(element *atom);         // depends on atomic_number
void get_configuration(element *atom);  // depends on atomic_number
void get_period(element *atom);         // depends on configuration
void get_group(element *atom);          // depends on configuration

void panic(const char *message) __attribute__((noreturn));  // panic!

#endif
