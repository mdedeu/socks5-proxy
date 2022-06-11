#include "sock_request_writing.h"

void sock_request_writing_arrival(unsigned state, struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    generate_request_answer((struct sock_request_message * )client_information->parsed_message, key);
    selector_set_interest(key->s, client_information->client_fd, OP_WRITE);
}

unsigned sock_request_writing_write_handler(struct selector_key * key){
    sock_client *client_data = (sock_client *) key->data;

    if (!buffer_can_read(client_data->write_buffer))
        return SOCK_REQUEST_WRITING;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < write_amount) // check if not connected
        return SOCK_REQUEST_WRITING;
    else
        return CONNECTED;
}


void sock_request_writing_departure(unsigned state, struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    close_sock_request_message((struct sock_request_message * )client_information->parsed_message);
    selector_set_interest(key->s, client_information->client_fd, OP_NOOP);
    selector_set_interest(key->s, client_information->origin_fd, OP_NOOP);
}