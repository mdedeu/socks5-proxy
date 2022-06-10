#ifndef COOL_REQUEST_READING_H
#define COOL_REQUEST_READING_H

#include "../cool_state_machine.h"

void cool_request_reading_departure(const unsigned int leaving_state, struct selector_key *key);
unsigned cool_request_read_handler(struct selector_key *key);
void cool_request_reading_arrival(const unsigned int leaving_state, struct selector_key *key);

#endif