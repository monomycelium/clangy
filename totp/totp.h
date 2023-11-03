#ifndef TOTP_H
#define TOTP_H

#include <stdint.h>
#include <time.h>

#define SEED_SIZE 20  // max length for decoded secret

typedef struct opts {
    time_t x;   // time steps in seconds (default: 30)
    time_t t0;  // Unix time to start counting time steps (default: 0)
} opts_t;

/// @brief Calculates TOTP using key and time.
/// @param key A null-terminated Base32-encoded secret.
/// @param time The time step (e.g. (time(NULL) - opts.t0) / opts.x).
/// @return The calculated six-digit TOTP value, (uint32_t)-1 if the key is
/// invalid, or (uint32_t)-2 if the HMAC(3) failed.
uint32_t totp_calc(const uint8_t *key, time_t time);

#endif
