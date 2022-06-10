#include "request_reading.h"
#define  MAX_READ_LENGTH 512


void request_reading_arrival(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data!=NULL){
        sock_client *client_data = (sock_client *) key->data;
        client_data->using_parser = init_sock_request_parser();
        client_data->parsed_message = init_sock_request_message();
        selector_set_interest_key(key,OP_READ);
    }
}


unsigned request_reading_read_handler(struct selector_key *key){
    if(key==NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;

    char temp_buffer[MAX_READ_LENGTH];

    int received_amount = recv(key->fd, temp_buffer, MAX_READ_LENGTH, MSG_DONTWAIT);
    if(received_amount <= 0 )
        return CLOSING_CONNECTION;


    struct sock_request_message * received_message = (struct sock_request_message *)client_data->parsed_message;
    if(received_message == NULL)
        return CLOSING_CONNECTION;

    bool finished = feed_sock_request_parser(
            client_data->using_parser,
            received_message,
            temp_buffer,
            received_amount
    );

    if(!finished)
        return SOCK_REQUEST_READING;

    if(received_message->atyp == DOMAIN_NAME)
        return RESOLVING_HOST_ADDRESS;
    else if(received_message->atyp == IPV4ADDRESS || received_message->atyp == IPV6ADDRESS)
        return ADDRESS_CONNECTING;
    else return CLOSING_CONNECTION;

}


void request_reading_departure(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data !=NULL){
        sock_client *client_data = (sock_client *) key->data;
        close_sock_request_parser(client_data->using_parser);
        selector_set_interest_key(key,OP_NOOP);
    }
}
