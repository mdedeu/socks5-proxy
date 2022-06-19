#ifndef COOL_HANDLERS_H
#define COOL_HANDLERS_H

#include "cool_client.h"
#include "state/cool_state_machine.h"
#include "cool_request_processor.h"
#include <stddef.h>

void cool_read(struct selector_key * key);
void cool_write(struct selector_key * key);
void cool_block(struct selector_key * key);
void cool_close(struct selector_key * key);
void cool_timeout(struct selector_key * key);

#endif
