//
// Created by shadad on 10/06/22.
//

#ifndef SOCKS5_PROXY_ADDRESS_CONNECTING_H
#define SOCKS5_PROXY_ADDRESS_CONNECTING_H
#include "proxy_state_machine.h"

void address_connecting_arrival(unsigned state, struct selector_key * key);
unsigned address_connecting_write_handler(struct selector_key * key);

#endif //SOCKS5_PROXY_ADDRESS_CONNECTING_H
