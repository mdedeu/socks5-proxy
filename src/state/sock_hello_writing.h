//
// Created by shadad on 10/06/22.
//

#ifndef SOCKS5_PROXY_SOCK_HELLO_WRITING_H
#define SOCKS5_PROXY_SOCK_HELLO_WRITING_H
#include "proxy_state_machine.h"

void sock_hello_writing_on_arrival(const unsigned state, struct selector_key *key);
unsigned sock_hello_write_handler(struct selector_key *key);
void sock_hello_writing_on_departure(const unsigned state , struct selector_key *key) ;

#endif //SOCKS5_PROXY_SOCK_HELLO_WRITING_H
