#include "json.h"

#define TOKENS 512

bool streq(const char *json, jsmntok_t *token, const char *string) {
    return (strlen(string) == (size_t)(token->end - token->start) &&
            !strncmp(json + token->start, string, token->end - token->start));
}

ssize_t puttoken(const char *json, jsmntok_t *token) {
    if (token->type == JSMN_STRING)
        return printf("%.*s\n", (int)(token->end - token->start),
                      json + token->start);
    else
        return -1;
}

void process(config *app) {
    jsmn_parser parser;
    jsmn_init(&parser);

    jsmntok_t tokens[TOKENS];
    size_t token_size = jsmn_parse(&parser, app->json_string.buf,
                                   app->json_string.strlen, tokens, TOKENS);

    switch (token_size) {
        case (size_t)JSMN_ERROR_INVAL:
            panic("invalid JSON");
        case (size_t)JSMN_ERROR_NOMEM:
            panic("not enough JSMN tokens allocated");
        case (size_t)JSMN_ERROR_PART:
            panic("partial JSON");
        default:
            break;
    }
    if (tokens->type != JSMN_OBJECT) panic("root is not an object");

    for (size_t i = 0, bus_service_count = 0; i < token_size; i++) {
        jsmntok_t *token = tokens + i;
        switch (token->type) {
            case JSMN_STRING:
                if (streq(app->json_string.buf, token, "ServiceNo")) {
                    if (bus_service_count > 0) putchar('\n');
                    puttoken(app->json_string.buf, token + 1);
                    bus_service_count++;
                } else if (streq(app->json_string.buf, token,
                                 "EstimatedArrival"))
                    print_next(app, token + 1);
                continue;
            case JSMN_ARRAY:
                for (size_t j = 0; j < (size_t)(tokens + i)->size; j++) {
                    if ((tokens + i + j + 1)->type != JSMN_OBJECT) continue;
                    for (size_t k = 0; k < (size_t)(tokens + i + j + 1)->size;
                         i++, k++) {
                    }
                }
            default:
                continue;
        }
    }
}
