/// influenced by
/// - http://www.64lines.com/jpeg-width-height
/// - https://stackoverflow.com/q/2517854

#include "jpegdump.h"

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MARKER_SOI_LEN 4
#define MARKER_SOI \
    (const uint8_t[]) { 0xFF, 0xD8, 0xFF, 0xE0 }
#define MARKER_HEADER_LEN 5
#define MARKER_HEADER \
    (const uint8_t[]) { 'J', 'F', 'I', 'F', 0 }

// get two bytes as one u16 byte
// i.e. (0xFF, 0xFF) -> 0xFFFF
static uint16_t length(const uint8_t *data, size_t i) {
    return htons(*(uint16_t *)(data + i));
}

// get jpeg metadata
// gets height and width (and jpeg validity).
jpeg_t jpegdump(const uint8_t *data, size_t len) {
    if (memcmp(data, MARKER_SOI, MARKER_SOI_LEN) > 0 ||
        memcmp(data + 6, MARKER_HEADER, MARKER_HEADER_LEN) > 0)
        return (jpeg_t){
            .err = JD_INVALID_JPEG};  // invalid data (or progressive JPEG)

    for (size_t i = 4 + length(data, 4); i < len;) {
        // check for invalid block
        if (data[i] != 0xFF) return (jpeg_t){.err = JD_INVALID_BLOCK};

        // if jpeg marker does not match start of frame,
        // skip the current "segment" by reading and skipping its length.
        // see https://en.wikipedia.org/wiki/JPEG#Syntax_and_structure.
        if (data[i + 1] != 0xC0 && data[i + 1] != 0xC2) {
            i += 2;
            i += length(data, i);
            continue;
        }

        // finally, return the metadata
        return (jpeg_t){
            .height = length(data, i + 5),
            .width = length(data, i + 7),
            .err = JD_SUCCESS,
        };
    }

    return (jpeg_t){.err = JD_MISSING_INFO};  // no size data found
}
