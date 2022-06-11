//
// Created by shadad on 10/06/22.
//

#ifndef SOCKS5_PROXY_SOCK_NEGATIVE_REQUEST_WRITING_H
#define SOCKS5_PROXY_SOCK_NEGATIVE_REQUEST_WRITING_H
#include "proxy_state_machine.h"

void sock_negative_request_writing_arrival(unsigned state, struct selector_key * key);
unsigned sock_negative_request_write_handler(struct selector_key * key);
void sock_negative_request_writing_departure(unsigned state, struct selector_key * key);
#endif //SOCKS5_PROXY_SOCK_NEGATIVE_REQUEST_WRITING_H
