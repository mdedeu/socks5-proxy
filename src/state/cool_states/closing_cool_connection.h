#ifndef CLOSING_COOL_CONNECTION_H
#define CLOSING_COOL_CONNECTION_H
#include "../cool_state_machine.h"
#include "../../cool_handlers.h"

void closing_cool_connection_on_arrival(const unsigned state, struct selector_key * key);

#endif