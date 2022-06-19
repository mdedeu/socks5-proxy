#include "cool_response_writing.h"

void cool_response_writing_arrival(const unsigned int leaving_state, struct selector_key *key){
    if(key == NULL)
        return;
    selector_set_interest_key(key,OP_WRITE);
}


unsigned cool_response_write_handler(struct selector_key *key){
    if(key == NULL || key->data == NULL)
        return CLOSING_COOL_CONNECTION;

    cool_client *client_data = (cool_client *) key->data;

    if (!buffer_can_read(client_data->write_buffer))
        return COOL_RESPONSE_WRITING;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    if(written_bytes < 0)
        return CLOSING_COOL_CONNECTION;
    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < (ssize_t)write_amount)
        return COOL_RESPONSE_WRITING;
    else
        return COOL_REQUEST_READING;
}

void cool_response_writing_departure(const unsigned int leaving_state, struct selector_key *key){
    if(key == NULL)
        return;
    selector_set_interest_key(key, OP_NOOP);
}