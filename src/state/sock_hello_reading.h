
#ifndef TCP_CONNECTED_H
#define TCP_CONNECTED_H

#include "proxy_state_machine.h"
void sock_hello_reading_on_arrival(const unsigned int leaving_state, struct selector_key *key);
 unsigned sock_hello_read_handler(struct selector_key *key);
 void sock_hello_reading_on_departure(const unsigned int leaving_state, struct selector_key *key);
#endif