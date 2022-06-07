#include "authenticated.h"


unsigned authenticated_handler_read(struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;

    if(!buffer_can_write(client_data->write_buffer))
        return AUTHENTICATED;

    size_t available_space;
    buffer_compact(client_data->write_buffer);
    uint8_t * writeFrom = buffer_write_ptr(client_data->write_buffer, &available_space);

    int received_amount = recv(key->fd, writeFrom, available_space, MSG_DONTWAIT);
    buffer_write_adv(client_data->write_buffer, received_amount);

    bool finished = feed_sock_request_parser(
        (struct sock_request_message *) (client_data->current_parser.request_message),
        writeFrom,
        received_amount
    );
    
    if(!finished){
        return AUTHENTICATED;
    }
    else {
        process_request_message(client_data->current_parser.request_message, key);
        return READY_TO_CONNECT;
    }
    return AUTHENTICATED;
}
