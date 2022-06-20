#include "sock_hello_reading.h"
#define SOCK_HELLO_MAX_LENGTH 512

void sock_hello_reading_on_arrival(const unsigned state, struct selector_key *key){
    if(key != NULL &&key->data != NULL){
        sock_client *client_data = (sock_client *) key->data;
        client_data->using_parser = init_sock_hello_parser();
        client_data->parsed_message = init_sock_hello_message();
        client_data->client_state = HELLO;
        selector_set_interest_key(key,OP_READ);
    }
}


unsigned sock_hello_read_handler(struct selector_key *key) {

    if(key==NULL || key->data ==NULL)
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;

    if(client_data->using_parser == NULL || client_data->parsed_message == NULL )
        return CLOSING_CONNECTION ;

    char receiving_buffer[SOCK_HELLO_MAX_LENGTH];
    int read_amount = recv(key->fd, receiving_buffer, SOCK_HELLO_MAX_LENGTH, MSG_DONTWAIT);

    if(read_amount <= 0 )
        return  CLOSING_CONNECTION;

    bool finished = feed_sock_hello_parser(
            client_data->using_parser,
        (struct sock_hello_message *) (client_data->parsed_message),
        receiving_buffer,
        read_amount
    );
    if(finished){
        client_data->close_after_write = !process_hello_message((struct sock_hello_message *) client_data->parsed_message, key);
        return SOCK_HELLO_WRITING ;
    }

    return  SOCK_HELLO_READING;
}



void sock_hello_reading_on_departure(const unsigned state , struct selector_key *key) {
    if(key != NULL  && key->data != NULL) {
        sock_client *client_data = (sock_client *) key->data;
        close_sock_hello_message((struct sock_hello_message *) client_data->parsed_message);
        close_sock_hello_parser(client_data->using_parser);
        client_data->parsed_message = NULL;
        client_data->using_parser = NULL;
        selector_set_interest_key(key, OP_NOOP);
    }
}


