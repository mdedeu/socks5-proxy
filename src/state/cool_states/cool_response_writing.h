#ifndef COOL_RESPONSE_WRITING_H
#define COOL_RESPONSE_WRITING_H
#include "../cool_state_machine.h"

void cool_response_writing_arrival(const unsigned int leaving_state, struct selector_key *key);
unsigned cool_response_write_handler(struct selector_key *key);
void cool_response_writing_departure(const unsigned int leaving_state, struct selector_key *key);

#endif
