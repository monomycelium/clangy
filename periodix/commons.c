#include "commons.h"

#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "element.h"

const char periodic_table[] =
    "               Periodic Table of Elements\n"
    "  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18\n"
    "1 H                                                  He\n"
    "2 Li Be                               B  C  N  O  F  Ne\n"
    "3 Na Mg                               Al Si P  S  Cl Ar\n"
    "4 K  Ca Sc Ti V  Cr Mn Fe Co Ni Cu Zn Ga Ge As Se Br Kr\n"
    "5 Rb Sr Y  Zr Nb Mo Tc Ru Rh Pd Ag Cd In Sn Sb Te I  Xe\n"
    "6 Cs Ba La Hf Ta W  Re Os Ir Pt Au Hg Tl Pb Bi Po At Rn\n"
    "7 Fr Ra Ac Rf Db Sg Bh Hs Mt Ds Rg Cn Nh Fl Mc Lv Ts Og\n"
    "\n"
    "        Ce Pr Nd Pm Sm Eu Gd Tb Dy Ho Er Tm Yb Lu\n"
    "        Th Pa U  Np Pu Am Cm Bk Cf Es Fm Md No Lr";

void panic(const char *message, ...) {
    va_list args;
    va_start(args, message);

    vfprintf(stderr, message, args);
    if (errno)
        fprintf(stderr, ": %s.\n", strerror(errno));
    else
        fprintf(stderr, ".\n");

    va_end(args);
    exit(errno);
}

option process_arguments(uint8_t argc, char **argv, element *atom) {
    const struct option long_options[] = {
        {"position", required_argument, 0, 'p'},
        {"name", required_argument, 0, 'n'},
        {"symbol", required_argument, 0, 's'},
    };

    option opt = NONE;
    int option = getopt_long(argc, argv, "p:n:s:", long_options, NULL);
    if (option <= -1) goto read;

    switch (option) {
        case 'p':
            opt = POSITION;
            sscanf(optarg, "%" SCNu8 ",%" SCNu8, &atom->group, &atom->period);
            goto check;
        case 'n':
            opt = NAME;
            atom->name = optarg;
            goto check;
        case 's':
            opt = SYMBOL;
            atom->symbol = optarg;
            goto check;
        default:
            panic("unknown option: %c", optopt);
    }

check:
    if (opt != NONE && optind < argc) panic("too many arguments");
    return opt;

read:
    if (optind >= argc) return NONE;
    if (optind + 1 < argc) panic("too many arguments");
    atom->atomic_number = atoi(argv[optind]);
    return NUMBER;
}
