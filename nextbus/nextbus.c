#include "commons.h"
#include "fetch.h"
#include "json.h"

int main(int argc, char **argv) {
    config app_config = {0};

    switch (argc) {
        case 3:
            app_config.bus_service = argv[2];
            if (strlen(argv[2]) > 4)
                panic("invalid bus service: maximum length is four");
        case 2:
            app_config.bus_stop = argv[1];
            if (strlen(argv[1]) != 5)
                panic("invalid bus stop: length should be five");
            break;
        default:
            fprintf(stderr, "usage: %s <BUS_STOP> [BUS_SERVICE]\n", argv[0]);
            return EXIT_FAILURE;
    }

    app_config.api_key = getenv("API_KEY");
    if (errno == EINVAL) panic("failed to get API key");
    if (app_config.api_key == NULL)
        panic(
            "please set `API_KEY` variable to LTA DataMall account key. see "
            "https://datamall.lta.gov.sg/ for more details");

    buffer_t buffer = fetch_data(app_config.bus_stop, app_config.bus_service,
                                 getenv("API_KEY"));
    process(buffer.buf, format);

    return EXIT_SUCCESS;
}
