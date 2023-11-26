#ifndef JPEGDUMP_H
#define JPEGDUMP_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    JD_SUCCESS = 0,
    JD_INVALID_BLOCK,
    JD_INVALID_JPEG,
    JD_MISSING_INFO,
} jpeg_error;

typedef struct {
    jpeg_error err;
    uint16_t height;
    uint16_t width;
} jpeg_t;

jpeg_t jpegdump(const uint8_t *data, size_t len);

#endif