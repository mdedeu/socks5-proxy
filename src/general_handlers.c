#include "general_handlers.h"

static void check_close_resources(sock_client * client_information,struct selector_key * key);

void socks5_read(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_read(associated_state_machine,key);
}
void socks5_write(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_write(associated_state_machine,key);
}
void socks5_block(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    stm_handler_block(associated_state_machine,key);
}
void socks5_close(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
   stm_handler_close(associated_state_machine,key);
}

