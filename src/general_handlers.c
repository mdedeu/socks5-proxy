#include "general_handlers.h"


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


void socks5_timeout(struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return;

    sock_client * client_information = (sock_client*) key->data;
    bool already_closed = false;

    if( client_information->origin_fd > 0){
        if(SELECTOR_SUCCESS == selector_unregister_fd(key->s,client_information->origin_fd) )
            close(client_information->origin_fd);
        else already_closed =true;
    }

    if( client_information->client_fd > 0) {
        if (SELECTOR_SUCCESS == selector_unregister_fd(key->s, client_information->client_fd))
            close(client_information->client_fd);
        else already_closed = true;

        if(client_information->username != NULL ) disconnect(client_information->username);

        if(!already_closed)
            destroy_sock_client(client_information);
    }


}

