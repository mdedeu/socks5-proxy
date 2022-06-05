#include "hello_sock_received.h"

 void on_hello_sock_departure(const unsigned int leaving_state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    close_sock_authentication_parser(client_data->current_parser.authentication_message);
    client_data->current_parser.request_message= init_sock_request_parser();
}



 unsigned hello_sock_received_handler_write(struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;
    if (!buffer_can_read(client_data->write_buffer))
        return HELLO_SOCK_RECEIVED;
    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);
    selector_set_interest_key(key, OP_READ);
    return AUTHENTICATED;
}


unsigned hello_sock_received_handler_read(struct selector_key *key){
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    size_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >=  AUTHENTICATION_ANSWER_LENGTH) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_authentication_parser((struct sock_authentication_message *) (client_data->current_parser.authentication_message), aux_buff,
                                                        read_amount);
        if(!finished)
            return HELLO_SOCK_RECEIVED;
        else {
            process_authentication_message(client_data->current_parser.authentication_message, key);
            selector_set_interest_key(key, OP_WRITE);
            return HELLO_SOCK_RECEIVED;
        }
    }
    return HELLO_SOCK_RECEIVED;
}


