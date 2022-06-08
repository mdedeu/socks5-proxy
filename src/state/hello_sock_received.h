#ifndef HELLO_SOCK_RECEIVED_H
#define HELLO_SOCK_RECEIVED_H
#include "proxy_state_machine.h"

void on_hello_sock_departure(const unsigned int leaving_state, struct selector_key *key);
unsigned hello_sock_received_handler_write(struct selector_key *key);
unsigned hello_sock_received_handler_read(struct selector_key *key);

#endif