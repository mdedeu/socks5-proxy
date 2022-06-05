#ifndef CONNECTED_H
#define
#include "proxy_state_machine.h"

void connected_on_arrival(unsigned  state , struct selector_key * key);
unsigned  connected_read_handler(struct  selector_key * key);
unsigned connected_write_handler(struct selector_key * key );

#endif