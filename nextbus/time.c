#include "time.h"

void print_next(const char *arrival) {
    if (strlen(arrival) != 25) return;

    struct tm arrival_time = {0};
    if (strptime(arrival, "%FT%T", &arrival_time) == NULL)
        panic("failed to parse time");
    arrival_time.tm_isdst = -1;
    arrival_time.tm_gmtoff = 28800;

    time_t diff = mktime(&arrival_time) - time(NULL);
    if (diff > 0)
        printf("%02u:%02u\n", (uint32_t)diff / 60, (uint32_t)diff % 60);
    else
        puts("arriving");
}
