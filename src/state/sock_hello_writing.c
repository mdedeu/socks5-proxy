#include "sock_hello_writing.h"

void sock_hello_writing_on_arrival(const unsigned state, struct selector_key *key){
    if(key!=NULL)
        selector_set_interest_key(key, OP_WRITE);
}


unsigned sock_hello_write_handler(struct selector_key *key) {
    if(key==NULL|| key->data==NULL)
        return CLOSING_CONNECTION;

    sock_client *client_data = (sock_client *) key->data;

    if( client_data->write_buffer == NULL || !buffer_can_read(client_data->write_buffer) )
        return CLOSING_CONNECTION ;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    if(written_bytes < 0)
        return CLOSING_CONNECTION;

    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < (ssize_t)write_amount)
        return SOCK_HELLO_WRITING;
    else if(client_data->close_after_write)
        return CLOSING_CONNECTION;
    else
        if(clients_need_authentication()){
            return SOCK_AUTHENTICATE_READING;
        }else return SOCK_REQUEST_READING;
}



void sock_hello_writing_on_departure(const unsigned state , struct selector_key *key) {
    if(key!=NULL)
        selector_set_interest_key(key, OP_NOOP);
}

