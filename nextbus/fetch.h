#include <curl/curl.h>

#include "commons.h"

typedef struct {
    char *buf;
    size_t bufsiz;
    size_t strlen;
} buffer_t;

buffer_t fetch_data(char *bus_stop, char *bus_service, char *api_key);
