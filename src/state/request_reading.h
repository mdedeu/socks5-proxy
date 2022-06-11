#ifndef AUTHENTICATED_H
#define AUTHENTICATED_H

#include "proxy_state_machine.h"

void request_reading_arrival(const unsigned state , struct selector_key * key );
unsigned request_reading_read_handler(struct selector_key *key);
void request_reading_departure(const unsigned state , struct selector_key * key );
unsigned request_reading_block_handler(struct selector_key *key);

#endif