#include "closing_connection.h"


void  closing_connection_on_departure(const unsigned state ,struct selector_key * key){
    if(key->fd > 0 )
        close(key->fd);
}

void closing_connection_on_arrival(const unsigned state ,struct selector_key * key ){
    selector_unregister_fd(key->s,key->fd);
    sock_client * client_information = (sock_client*) key->data;
    if(key->fd == client_information->origin_fd)
        selector_unregister_fd(key->s,client_information->client_fd);
    else
        selector_unregister_fd(key->s,client_information->origin_fd);
    destroy_sock_client(client_information);
}