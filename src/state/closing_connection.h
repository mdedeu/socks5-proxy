#ifndef CLOSING_CONNECTION_H
#define CLOSING_CONNECTION_H
#include "proxy_state_machine.h"
#include "../general_handlers.h"

void closing_connection_on_arrival(const unsigned state ,struct selector_key * key );

#endif