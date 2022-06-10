#ifndef COOL_AUTHENTICATE_READING_H
#define COOL_AUTHENTICATE_READING_H
#include "../cool_state_machine.h"

void authenticate_reading_departure(const unsigned int leaving_state, struct selector_key *key);
unsigned authenticate_read_handler(struct selector_key *key);
void authenticate_reading_arrival(const unsigned int leaving_state, struct selector_key *key);

#endif