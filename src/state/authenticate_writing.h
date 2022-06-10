//
// Created by shadad on 10/06/22.
//

#ifndef SOCKS5_PROXY_AUTHENTICATE_WRITING_H
#define SOCKS5_PROXY_AUTHENTICATE_WRITING_H
#include "proxy_state_machine.h"

void authenticate_writing_arrival(const unsigned int leaving_state, struct selector_key *key);
unsigned authenticate_write_handler(struct selector_key *key);
void authenticate_writing_departure(const unsigned int leaving_state, struct selector_key *key);

#endif //SOCKS5_PROXY_AUTHENTICATE_WRITING_H
