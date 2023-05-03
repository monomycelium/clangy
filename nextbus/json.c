#include "json.h"

// What is this? 
void format(json_t *nextbus) {
    json_t *arrival = json_object_get(nextbus, "EstimatedArrival");
    if (!json_is_string(arrival)) panic("failed to read `EstimatedArrival`");
    print_next(json_string_value(arrival));
}

void process(char *js, void (*callback)(json_t *)) {
    json_error_t error;
    json_t *root = json_loads(js, 0, &error);
    free(js);

    if (!root) {
        fprintf(stderr, "failed to parse JSON: (data.json:%d): %s\n",
                error.line, error.text);
        exit(EXIT_FAILURE);
    }

    json_t *bus_stop = json_object_get(root, "BusStopCode");
    if (!json_is_string(bus_stop)) panic("failed to read `BusStopCode`");

    json_t *services = json_object_get(root, "Services");
    if (!json_is_array(services)) panic("failed to read `Services`");

    for (size_t i = 0; i < json_array_size(services); i++) {
        json_t *service = json_array_get(services, i);

        json_t *service_number = json_object_get(service, "ServiceNo");
        if (!json_is_string(service_number))
            panic("failed to read `ServiceNo`");
        puts(json_string_value(service_number));

        char *nextbuses[3] = {"NextBus", "NextBus2", "NextBus3"};
        for (uint8_t i = 0; i < 3; i++) {
            callback(json_object_get(service, nextbuses[i]));
        }
    }

    json_decref(root);
}
