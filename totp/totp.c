#include "totp.h"

#include <endian.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "base32.h"

uint32_t totp_calc(const uint8_t *k, time_t t) {
    uint8_t seed[SEED_SIZE];  // seed for HMAC-SHA1
    int seedlen;              // length of decoded secret
    uint8_t *hmac;            // HMAC(3) result
    uint32_t hmaclen;         // length of HMAC(3) result
    uint32_t totp;            // calculated TOTP
    uint32_t bin_code;
    size_t offset;

    t = ((time_t)-1 == (time_t)UINT32_MAX) ? htobe32((uint32_t)t)
                                           : htobe64((uint64_t)t);

    seedlen = base32_decode(k, seed, sizeof(seed));
    if (seedlen == -1) return (uint32_t)-1;

    hmac = HMAC(EVP_sha1(), seed, (size_t)seedlen, (const uint8_t *)&t,
                sizeof(t), NULL, &hmaclen);
    if (hmac == NULL) return (uint32_t)-2;

    offset = hmac[hmaclen - 1] & 0xf;
    bin_code = (hmac[offset] & 0x7f) << 24 | (hmac[offset + 1] & 0xff) << 16 |
               (hmac[offset + 2] & 0xff) << 8 | (hmac[offset + 3] & 0xff);

    totp = bin_code % 1000000;
    return totp;
}
