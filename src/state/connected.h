#ifndef CONNECTED_H
#define CONNECTED_H

#include "proxy_state_machine.h"
#include "pop3_dissector.h"

void connected_on_arrival(unsigned  state , struct selector_key * key);
unsigned  connected_read_handler(struct  selector_key * key);
unsigned connected_write_handler(struct selector_key * key );
void connected_on_departure(unsigned state, struct selector_key * key);

#endif