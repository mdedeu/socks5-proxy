#include "closing_cool_connection.h"



void closing_cool_connection_on_arrival(const unsigned state, struct selector_key * key){
    if(key != NULL && key->s != NULL)
        selector_unregister_fd(key->s,key->fd);

    if(key->data == NULL)
        return;

    cool_client * client_information = (cool_client*) key->data;
    selector_unregister_fd(key->s,client_information->client_fd);

    if(client_information->client_fd > 0 )
        close(client_information->client_fd);
    destroy_cool_client(client_information);
}