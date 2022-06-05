#ifndef CLOSING_CONNECTION_H
#define
#include "proxy_state_machine.h"

unsigned  closing_connection_on_departure(const unsigned state ,struct selector_key * key);
static unsigned closing_connection_on_arrival(const unsigned state ,struct selector_key * key );

#endif