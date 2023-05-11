#ifndef COMMONS_H
#define COMMONS_H

#include "element.h"

extern const char periodic_table[];

void panic(const char *message, ...) __attribute__((noreturn));
option process_arguments(uint8_t argc, char **argv, element *atom);

#endif
