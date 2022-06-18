#include "closing_connection.h"



void closing_connection_on_arrival(const unsigned state ,struct selector_key * key ){
//    sock_client * client_information = (sock_client*) key->data;
//    bool already_closed = false;
//
//    if( client_information->origin_fd > 0  && SELECTOR_SUCCESS == selector_unregister_fd(key->s,client_information->origin_fd) ) {
//        close(client_information->origin_fd);
//    }else already_closed = true;
//
//    if( client_information->client_fd > 0  && SELECTOR_SUCCESS == selector_unregister_fd(key->s,client_information->client_fd) ) {
//        close(client_information->client_fd);
//    } else already_closed=true;
//
//    if(client_information->username != NULL ) disconnect(client_information->username);
//
//    if(!already_closed)
//        destroy_sock_client(client_information);
    socks5_timeout(key);
}