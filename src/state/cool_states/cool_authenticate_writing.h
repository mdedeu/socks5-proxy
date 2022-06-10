#ifndef COOL_AUTHENTICATE_WRITING_H
#define COOL_AUTHENTICATE_WRITING_H
#include "../cool_state_machine.h"

void cool_authenticate_writing_arrival(const unsigned int leaving_state, struct selector_key *key);
unsigned cool_authenticate_write_handler(struct selector_key *key);
void cool_authenticate_writing_departure(const unsigned int leaving_state, struct selector_key *key);

#endif
