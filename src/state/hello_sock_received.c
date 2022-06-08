#include "hello_sock_received.h"

 void on_hello_sock_departure(const unsigned int leaving_state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;

    close_sock_authentication_parser(client_data->current_parser.authentication_message);
    client_data->current_parser.request_message = init_sock_request_parser();
}

//PREG_SALTA: me parece raro que usemos el write_buffer para escribirle al cliente, mirar eso
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
    sock_client * client_data = (sock_client *) key->data;

    buffer_compact(client_data->write_buffer);
    if(!buffer_can_write(client_data->write_buffer))
        return HELLO_SOCK_RECEIVED;

    size_t available_space;
    uint8_t * write_from = buffer_write_ptr(client_data->write_buffer, &available_space);

    int received_amount = recv(key->fd, write_from, available_space, MSG_DONTWAIT);
    buffer_write_adv(client_data->write_buffer, received_amount);

    uint8_t * read_from = buffer_read_ptr(client_data->write_buffer, &available_space);

    bool finished = feed_sock_authentication_parser(
        (struct sock_authentication_message *) (client_data->current_parser.authentication_message),
        (char *) read_from,
        available_space 
    );

    //PREG_SALTA: como se que se esta consumiendo todo el available space?
    buffer_read_adv(client_data->write_buffer, available_space);
    buffer_compact(client_data->write_buffer);

    if(finished){
        process_authentication_message(client_data->current_parser.authentication_message, key);
        selector_set_interest_key(key, OP_WRITE);
    }

    return HELLO_SOCK_RECEIVED;
}


