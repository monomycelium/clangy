#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

float convert(char *str) {
    char *e;
    float f;

    errno = 0;

    f = strtof(str, &e);

    if (errno != 0 || str == e) {
        return 0.0;
    } else {
        return f;
    }
}

int main(int argc, char **argv) {
    unsigned short celsius = 2;
    float temperature;

    short option;
    int option_index = 0;

    static struct option long_options[] = {
        {"celsius", required_argument, 0, 'c'},
        {"fahrenheit", required_argument, 0, 'f'},
    };

    while ((option = getopt_long(argc, argv, "c:f:", long_options, &option_index)) > -1) {
        switch(option) {
            case 'f':
                temperature = convert(optarg);
                celsius = 0;
                // printf("bro got fahrenheit-ed: %f.\n", temperature);
                break;
            case 'c':
                temperature = convert(optarg);
                celsius = 1;
                // printf("finally metric: %f!\n", temperature);
                break;
            case '?':
                fprintf(stderr, "invalid option: %c.\n", optopt);
                return 1;
            default:
                fprintf(stderr, "unknown option: %c.\n", option);
                return 1;
        }
    }

    if (celsius == 2) {
        fprintf(stderr, "at least one flag required.\n");
        return 1;
    } else if (celsius) {
        printf("%.1f°F\n", temperature * 9 / 5 + 32);
    } else {
        printf("%.1f°C\n", (temperature - 32) * 5 / 9);
    }

    return 0;
}
