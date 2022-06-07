#include "authenticated.h"


unsigned authenticated_handler_read(struct selector_key *key){
    sock_client *client_data = (sock_client *) key->data;

    buffer_compact(client_data->write_buffer);
    if(!buffer_can_write(client_data->write_buffer))
        return AUTHENTICATED;

    size_t available_space;
    uint8_t * write_from = buffer_write_ptr(client_data->write_buffer, &available_space);

    int received_amount = recv(key->fd, write_from, available_space, MSG_DONTWAIT);

    bool finished = feed_sock_request_parser(
        (struct sock_request_message *) (client_data->current_parser.request_message),
        write_from,
        received_amount
    );

    //TODO: mirar que hacer en el caso en el que el cliente escriba de mas y este moviendo mal el puntero de lectura
    buffer_write_adv(client_data->write_buffer, received_amount);
    buffer_read_adv(client_data->write_buffer, received_amount);
    buffer_compact(client_data->write_buffer);
    
    if(!finished)
        return AUTHENTICATED;

    process_request_message(client_data->current_parser.request_message, key);
    return READY_TO_CONNECT;
}
