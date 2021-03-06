#include "cool_authenticate_writing.h"

void cool_authenticate_writing_arrival(const unsigned int leaving_state, struct selector_key *key){
    if(key == NULL)
        return;
    selector_set_interest_key(key,OP_WRITE);
}


unsigned cool_authenticate_write_handler(struct selector_key *key){
    if(key == NULL || key->data == NULL)
        return CLOSING_COOL_CONNECTION;
    cool_client *client_data = (cool_client *) key->data;

    if(client_data->write_buffer == NULL)
        return CLOSING_COOL_CONNECTION; 

    if (!buffer_can_read(client_data->write_buffer))
        return COOL_AUTHENTICATE_WRITING;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    if(written_bytes < 0)
        return CLOSING_COOL_CONNECTION;

    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < (ssize_t)write_amount)
        return COOL_AUTHENTICATE_WRITING;
    else if(*reading_since==0x4B && *(reading_since+1)==0x1D)
        return COOL_AUTHENTICATE_READING;
    else if(*reading_since==0xC0 && *(reading_since+1)==0x01)
        return COOL_REQUEST_READING;
    else
        return CLOSING_COOL_CONNECTION;
}

void cool_authenticate_writing_departure(const unsigned int leaving_state, struct selector_key *key){
    if(key == NULL)
        return;
    selector_set_interest_key(key,OP_NOOP);
}