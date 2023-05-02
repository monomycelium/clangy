#ifndef COMMONS_H
#define COMMONS_H

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    //     bool detailed;
    //     bool refresh;
    char *bus_stop;
    char *bus_service;
    char *api_key;
} config;

void panic(const char *message);

#endif
