#ifndef RESOLVING_HOST_ADDRESS_H
#define RESOLVING_HOST_ADDRESS_H

#include "proxy_state_machine.h"
#include "../dns_query.h"


unsigned resolving_host_address_block(struct selector_key *key);
void  resolving_host_address_arrival(const unsigned state, struct selector_key *key);

#endif