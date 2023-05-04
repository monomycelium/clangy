#include "fetch.h"

#define URL_FORMAT                                     \
    "http://datamall2.mytransport.sg/ltaodataservice/" \
    "BusArrivalv2?BusStopCode=%s"
#define URL_FORMAT_SERVICE "&ServiceNo=%s"

size_t process_data(char *buf, size_t size, size_t nmemb, void *data) {
    size_t bytes = size * nmemb;
    buffer_t *buffer = (buffer_t *)data;

    if (buffer->strlen == 0 && bytes <= 136) {
        fprintf(stderr, "bus stop not found.\n");
        exit(EXIT_FAILURE);
    };

    if (buffer->buf == NULL) {
        buffer->bufsiz = bytes;
        buffer->buf = malloc(buffer->bufsiz);
        if (buffer->buf == NULL) panic("failed to allocate memory");

#if defined(DEBUG)
        printf("did a malloc: %zu!\n", bytes);
#endif
    } else {
        buffer->bufsiz += bytes;
        buffer->buf = realloc(buffer->buf, buffer->bufsiz);
        if (buffer->buf == NULL) panic("failed to reallocate memory");

#if defined(DEBUG)
        printf("did a realloc: %zu!\n", bytes);
#endif
    }

    memcpy(buffer->buf + buffer->strlen, buf, bytes);
    buffer->strlen += bytes;

    return bytes;
}

buffer_t fetch_data(config *app) {
    char url[93];
    if (app->bus_service) {
        sprintf(url, URL_FORMAT URL_FORMAT_SERVICE, app->bus_stop,
                app->bus_service);
    } else
        sprintf(url, URL_FORMAT, app->bus_stop);

    char header[37];
    sprintf(header, "AccountKey: %s", app->api_key);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header);

    CURL *curl = curl_easy_init();
    if (!curl) panic("failed to initialise curl");

    buffer_t buffer = {NULL, 0, 0};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        fprintf(stderr, "failed to perform curl: %s.\n",
                curl_easy_strerror(result));
        exit(EXIT_FAILURE);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    return buffer;
}
