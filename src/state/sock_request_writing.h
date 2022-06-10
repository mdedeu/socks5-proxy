#ifndef WRITING_REPLY_H
#define WRITING_REPLY_H
#include "proxy_state_machine.h"

void sock_request_writing_arrival(unsigned state , struct selector_key * key );
unsigned  sock_request_writing_write_handler(struct selector_key * key);
void sock_request_writing_departure(unsigned state, struct selector_key * key);


#endif