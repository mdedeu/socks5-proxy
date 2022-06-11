#include "cool_handlers.h"

void cool_read(struct selector_key * key){
    cool_client * current_client = (cool_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_read(associated_state_machine,key);
}

void cool_write(struct selector_key * key){
    cool_client * current_client = (cool_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_write(associated_state_machine,key);
}

void cool_block(struct selector_key * key){
    cool_client * current_client = (cool_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_block(associated_state_machine,key);
}

void cool_close(struct selector_key * key){
    cool_client * current_client = (cool_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_close(associated_state_machine,key);
}

