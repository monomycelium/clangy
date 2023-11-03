/// regional
///
/// a CLI tool to encodes a string using regional indicator symbols.
/// concepts: pointers, ascii

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_USAGE "usage: %s <STR>\n"

// replace 4th byte with lowercase uint8_tacter + 0x45.
static uint8_t symbol[5] = {'\xf0', '\x9f', '\x87', '\000', '\000'};

/// Encodes uint8_tacter to regional indicator symbol letter.
/// See https://en.wikipedia.org/wiki/Regional_indicator_symbol.
uint8_t *regional(uint8_t c) {
    // if (!isalpha(c)) return NULL;
    symbol[3] = (c & 0x1F) + 0xa5;
    return symbol;
}

int main(int argc, char **argv) {
    uint8_t *code;  // original string

    if (argc != 2) {
        fprintf(stderr, ERROR_USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    code = (uint8_t *)argv[1];
    for (uint8_t i = 0; code[i] != 0; i++) switch (code[i]) {
            case 'a' ... 'z':
            case 'A' ... 'Z':
                fputs((const char *)regional(code[i]), stdout);
                continue;
            default:
                putchar(code[i]);
                continue;
        }

    putchar('\n');
    return EXIT_SUCCESS;
}
