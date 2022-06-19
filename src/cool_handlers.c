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

void cool_timeout(struct selector_key * key){
    if(key != NULL && key->s != NULL)
        selector_unregister_fd(key->s,key->fd);

    if(key->data == NULL)
        return;

    cool_client * client_information = (cool_client*) key->data;
    selector_unregister_fd(key->s,client_information->client_fd);

    if(client_information->parsed_message != NULL && client_information->state == COOL_CONNECTED)
        destroy_general_request_message((struct general_request_message * ) client_information->parsed_message);
    
    if(client_information->parsed_message != NULL && client_information->state == COOL_AUTHENTICATING)
        close_cool_protocol_authentication_message((struct cool_protocol_authentication_message * ) client_information->parsed_message);

    if(client_information->using_parser != NULL)
        close_cool_protocol_authentication_parser(client_information->using_parser);
    

    if(client_information->client_fd > 0 )
        close(client_information->client_fd);
    destroy_cool_client(client_information);
}

