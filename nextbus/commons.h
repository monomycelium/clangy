#ifndef COMMONS_H
#define COMMONS_H

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char *buf;
    size_t bufsiz;
    size_t strlen;
} buffer_t;

typedef struct {
    //     bool detailed;
    //     bool refresh;
    char *bus_stop;
    char *bus_service;
    char *api_key;
    buffer_t json_string;
    time_t current_time;
    //     uint8_t bus_service_count;
    //     uint8_t bus_arrival_count;
} config;

void panic(const char *message) __attribute__((noreturn));

#endif
