#include <jansson.h>

#include "commons.h"
#include "time.h"

void format(json_t *nextbus);
void process(char *js, void (*callback)(json_t *));
