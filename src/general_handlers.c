#include "general_handlers.h"
#include "sock_client.h"
#include "proxy_state_machine.c"

void socks5_read(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    const enum sock_state new_state = stm_handler_read(associated_state_machine,key);
    //check for error
}
void socks5_write(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    const enum sock_state new_state = stm_handler_write(associated_state_machine,key);
    //check for error
}
void socks5_block(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    const enum sock_state new_state = stm_handler_block(associated_state_machine,key);
    //check for error
}
void socks5_close(struct selector_key * key){
    sock_client * current_client = (sock_client* ) key->data;
    struct state_machine * associated_state_machine = current_client->client_state_machine ;
    const enum sock_state new_state = stm_handler_close(associated_state_machine,key);
    //check for error
}
