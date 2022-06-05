#ifndef WRITING_REPLY_H
#define WRITING_REPLY_H
#include "proxy_state_machine.h"

void writing_reply_on_arrival(unsigned state , struct selector_key * key );
unsigned  writing_reply_write_handler(struct selector_key * key);

#endif