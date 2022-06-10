#include "authenticate_reading.h"
#define SOCK_AUTHENTICATION_MAX_LENGTH 2
#define  MAX_READ_LENGTH 512

void authenticate_reading_arrival(const unsigned int leaving_state, struct selector_key *key){
    if(key != NULL &&key->data != NULL) {
        sock_client *client_data = (sock_client *) key->data;
        client_data->using_parser = init_sock_authentication_parser();
        client_data->parsed_message = init_sock_authentication_message();
        selector_set_interest_key(key, OP_READ);
    }
}

unsigned authenticate_read_handler(struct selector_key *key){
    if(key==NULL || key->data ==NULL)
        return CLOSING_CONNECTION;
    sock_client * client_data = (sock_client *) key->data;
    if(client_data->using_parser == NULL || client_data->parsed_message == NULL )
        return CLOSING_CONNECTION ;

    char temp_buffer[MAX_READ_LENGTH];

    int received_amount = recv(key->fd, temp_buffer, MAX_READ_LENGTH, MSG_DONTWAIT);

    if(received_amount <= 0){
        return CLOSING_CONNECTION;
    }


    bool finished = feed_sock_authentication_parser(
            client_data->using_parser,
        (struct sock_authentication_message *) (client_data->parsed_message),
            temp_buffer,
            received_amount
    );

    if(finished){
        client_data->close_after_write = !process_authentication_message((struct sock_authentication_message *) client_data->parsed_message, key);
        return SOCK_AUTHENTICATE_WRITING;
    }
    return SOCK_AUTHENTICATE_READING;
}


void authenticate_reading_departure(const unsigned int leaving_state, struct selector_key *key){
    if(key != NULL &&key->data != NULL) {
        sock_client *client_data = (sock_client *) key->data;
        close_sock_authentication_parser(client_data->using_parser);
        close_sock_authentication_message((struct sock_authentication_message *) client_data->parsed_message);
        selector_set_interest_key(key, OP_NOOP);
    }
}



