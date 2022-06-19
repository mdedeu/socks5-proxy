//
// Created by shadad on 10/06/22.
//

#ifndef SOCKS5_PROXY_DOMAIN_CONNECTING_H
#define SOCKS5_PROXY_DOMAIN_CONNECTING_H
#include "proxy_state_machine.h"

void domain_connecting_arrival(unsigned state, struct selector_key * key);

unsigned domain_connecting_write_handler(struct selector_key * key);


#endif //SOCKS5_PROXY_DOMAIN_CONNECTING_H
