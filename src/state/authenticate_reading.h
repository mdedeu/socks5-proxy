#ifndef HELLO_SOCK_RECEIVED_H
#define HELLO_SOCK_RECEIVED_H
#include "proxy_state_machine.h"

void authenticate_reading_departure(const unsigned int leaving_state, struct selector_key *key);
unsigned authenticate_read_handler(struct selector_key *key);
void authenticate_reading_arrival(const unsigned int leaving_state, struct selector_key *key);

#endif