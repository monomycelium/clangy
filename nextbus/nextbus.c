#include "commons.h"
#include "fetch.h"
#include "json.h"

int main(int argc, char **argv) {
    config app = {0};

    switch (argc) {
        case 3:
            if (strlen(argv[2]) > 4)
                panic("invalid bus service: maximum length is four");
            app.bus_service = argv[2];
            __attribute__((fallthrough));
        case 2:
            if (strlen(argv[1]) != 5)
                panic("invalid bus stop: length should be five");
            app.bus_stop = argv[1];
            break;
        default:
            fprintf(stderr, "usage: %s <BUS_STOP> [BUS_SERVICE]\n", argv[0]);
            return EXIT_FAILURE;
    }

    app.api_key = getenv("API_KEY");
    if (errno == EINVAL) panic("failed to get API key");
    if (app.api_key == NULL)
        panic(
            "please set `API_KEY` variable to LTA DataMall account key. see "
            "https://datamall.lta.gov.sg/ for more details");

    app.current_time = time(NULL);
    app.json_string = fetch_data(&app);
    process(&app);

    return EXIT_SUCCESS;
}
