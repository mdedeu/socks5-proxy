#include "address_connecting.h"


unsigned address_connecting_write_handler(struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    struct sock_client * client_information = (struct sock_client *) key->data;
    struct sock_request_message * client_message = (struct sock_request_message * )client_information->parsed_message;
    if(client_message == NULL )
        return CLOSING_CONNECTION;

    int error ;
    socklen_t  length = sizeof (error);
    if( getsockopt(client_information->origin_fd, SOL_SOCKET, SO_ERROR, &error, &length) < 0 ){
        client_message->connection_result = status_general_socks_server_failure;
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }else if(error == 0 ){
        return SOCK_REQUEST_WRITING;
    }else{
        client_message->connection_result =  errno_to_sock(error);
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }


}

void address_connecting_departure(const unsigned int leaving_state, struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return;
    struct sock_client * client_information = (struct sock_client *) key->data;
    selector_set_interest(key->s,client_information->origin_fd , OP_NOOP);
    selector_set_interest(key->s,client_information->client_fd,OP_NOOP);
}





