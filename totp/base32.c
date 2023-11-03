// Base32 implementation
//
// Copyright 2010 Google Inc.
// Author: Markus Gutschke
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "base32.h"

#include <string.h>

int base32_decode(const uint8_t *encoded, uint8_t *result, int size) {
    int buffer, bitsLeft, count;
    buffer = bitsLeft = count = 0;

    for (const uint8_t *ptr = encoded; count < size && *ptr; ++ptr) {
        uint8_t ch = *ptr;
        buffer <<= 5;

        switch (ch) {
            case 'A' ... 'Z':
            case 'a' ... 'z':
                ch = (ch & 0x1F) - 1;
                break;
            case '2' ... '7':
                ch -= '2' - 26;
                break;
            default:
                return -1;
        }

        buffer |= ch;
        bitsLeft += 5;

        if (bitsLeft >= 8) {
            result[count++] = buffer >> (bitsLeft - 8);
            bitsLeft -= 8;
        }
    }

    if (count < size) result[count] = '\0';
    return count;
}
