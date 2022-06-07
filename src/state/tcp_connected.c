#include "tcp_connected.h"

unsigned on_tcp_connected_handler_read(struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;

    buffer_compact(client_data->write_buffer);
    if(!buffer_can_write(client_data->write_buffer))
        return TCP_CONNECTED;
    
    size_t available_space;
    uint8_t * write_from = buffer_write_ptr(client_data->write_buffer, &available_space);

    int read_amount = recv(key->fd, write_from, available_space, MSG_DONTWAIT);
    buffer_write_adv(client_data->write_buffer, read_amount);

    uint8_t * read_from = buffer_read_ptr(client_data->write_buffer, &available_space);    

    bool finished = feed_sock_hello_parser(
        (struct sock_hello_message *) (client_data->current_parser.hello_message),
        (char *) read_from,
        available_space
    );

    buffer_read_adv(client_data->write_buffer, read_amount);
    buffer_compact(client_data->write_buffer);

    if(finished){
        process_hello_message(client_data->current_parser.hello_message, key);
        selector_set_interest_key(key, OP_WRITE);
    }

    return TCP_CONNECTED;
}


//return the new state if corresponding
unsigned on_tcp_connected_handler_write(struct selector_key *key) {
    sock_client *client_data = (sock_client *) key->data;

    if (!buffer_can_read(client_data->write_buffer))
        return TCP_CONNECTED;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);

    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    //PREG_SALTA: como sabemos que se mando todo?
    selector_set_interest_key(key, OP_READ);
    return HELLO_SOCK_RECEIVED;
}

void on_tcp_connected_departure(const unsigned int leaving_state, struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;

    //PREG_STALTA: por que estaria cerrando el hello parser y abriendo el authentication parser si esto no sigue el orden de los estados?
    close_sock_hello_parser(client_data->current_parser.hello_message);
    client_data->current_parser.authentication_message = init_sock_authentication_parser();
}
