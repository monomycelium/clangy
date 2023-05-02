#include "fetch.h"

#define URL_FORMAT                                     \
    "http://datamall2.mytransport.sg/ltaodataservice/" \
    "BusArrivalv2?BusStopCode=%s"
#define URL_FORMAT_SERVICE "&ServiceNo=%s"

size_t process_data(char *buf, size_t size, size_t nmemb, void *data) {
    size_t bytes = size * nmemb;

    buffer_t *buffer = (buffer_t *)data;
    if (buffer->bufsiz - buffer->strlen < bytes) {
        buffer->bufsiz += bytes - (buffer->bufsiz - buffer->strlen);
        printf("did a malloc: %zu!\n", bytes);
        buffer->buf = realloc(buffer->buf, buffer->bufsiz);
        if (buffer->buf == NULL) panic("failed to reallocate memory");
    }

    memcpy(buffer->buf + buffer->strlen, buf, bytes);
    buffer->strlen += bytes;

    return bytes;
}

buffer_t fetch_data(char *bus_stop, char *bus_service, char *api_key) {
    char url[93];
    if (bus_service)
        sprintf(url, URL_FORMAT URL_FORMAT_SERVICE, bus_stop, bus_service);
    else
        sprintf(url, URL_FORMAT, bus_stop);

    char header[37];
    sprintf(header, "AccountKey: %s", api_key);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, header);

    CURL *curl = curl_easy_init();
    if (!curl) panic("failed to initialise curl");

    buffer_t buffer = {malloc(sizeof(char) * 800), 800, 0};
    if (buffer.buf == NULL) panic("failed to allocate memory");

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
