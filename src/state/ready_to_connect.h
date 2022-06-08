#ifndef READY_TO_CONNECT_H
#define READY_TO_CONNECT_H
#include "proxy_state_machine.h"

void ready_to_connect_on_arrival(unsigned state , struct selector_key * key );
unsigned ready_to_connect_block_handler(struct selector_key * key);
unsigned  ready_to_connect_write_handle(struct selector_key * key);


#endif
