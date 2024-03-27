/**
 * stdntn
 *
 * a CLI tool to express numeric values in standard notation.
 * concepts: algorithms
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    double a;
    intmax_t n;
} StdNtn;

StdNtn express(double x) {
    StdNtn ret;
    bool neg;

    neg = x < 0;
    if (neg) x = -x;

    ret.a = x;
    ret.n = 0;

    if (x == 0) return ret;

    while (ret.a < 1) {
        ret.a *= 10;
        ret.n--;
    }

    while (ret.a >= 10) {
        ret.a /= 10;
        ret.n++;
    }

    if (neg) ret.a = -ret.a;
    return ret;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <DOUBLE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double dub = atof(argv[1]);
    StdNtn n = express(dub);
    printf("express: %f * 10^%jd\n", n.a, n.n);
    printf("stdlib:  %e\n", dub);
    return EXIT_SUCCESS;
}
