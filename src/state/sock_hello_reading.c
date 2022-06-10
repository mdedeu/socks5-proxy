#include "sock_hello_reading.h"
#define SOCK_HELLO_MAX_LENGTH 2

void sock_hello_reading_on_arrival(const unsigned state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    client_data->using_parser = init_sock_hello_parser();
    client_data->parsed_message = init_sock_hello_message();
    selector_set_interest_key(key,OP_READ);
}


unsigned sock_hello_read_handler(struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;
    char receiving_buffer[SOCK_HELLO_MAX_LENGTH];

    int read_amount = recv(key->fd, receiving_buffer, SOCK_HELLO_MAX_LENGTH, MSG_DONTWAIT);


    bool finished = feed_sock_hello_parser(
            client_data->using_parser,
        (struct sock_hello_message *) (client_data->parsed_message),
        receiving_buffer,
        read_amount
    );

    return finished ? SOCK_HELLO_WRITING : SOCK_HELLO_READING;
}



void sock_hello_reading_on_departure(const unsigned state , struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;
    process_hello_message((struct sock_hello_message * )client_data->parsed_message,  key);
    close_sock_hello_message((struct sock_hello_message * )client_data->parsed_message);
    close_sock_hello_parser(client_data->using_parser);
    selector_set_interest_key(key, OP_NOOP);
}


