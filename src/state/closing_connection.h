#ifndef CLOSING_CONNECTION_H
#define CLOSING_CONNECTION_H
#include "proxy_state_machine.h"

unsigned  closing_connection_on_departure(const unsigned state ,struct selector_key * key);
unsigned closing_connection_on_arrival(const unsigned state ,struct selector_key * key );

#endif