
#ifndef TCP_CONNECTED_H
#define TCP_CONNECTED_H

#include "proxy_state_machine.h"
 unsigned on_tcp_connected_handler_read(struct selector_key *key);
 unsigned on_tcp_connected_handler_write(struct selector_key *key);
 void on_tcp_connected_departure(const unsigned int leaving_state, struct selector_key *key);
#endif