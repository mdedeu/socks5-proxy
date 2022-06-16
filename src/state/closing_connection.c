#include "closing_connection.h"



void closing_connection_on_arrival(const unsigned state ,struct selector_key * key ){
    selector_unregister_fd(key->s,key->fd);
    sock_client * client_information = (sock_client*) key->data;
    if(client_information->username != NULL ) disconnect(client_information->username);
    if(key->fd == client_information->origin_fd)
        selector_unregister_fd(key->s,client_information->client_fd);
    else
        selector_unregister_fd(key->s,client_information->origin_fd);

    if(client_information->client_fd > 0 )
        close(client_information->client_fd);
    if(client_information->origin_fd > 0 )
        close(client_information->origin_fd);
    destroy_sock_client(client_information);
}