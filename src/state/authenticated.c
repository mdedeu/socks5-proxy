#include "authenticated.h"


unsigned authenticated_handler_read(struct selector_key *key){
    char aux_buff[READ_AMOUNT];
    sock_client *client_data = (sock_client *) key->data;
    size_t available_space;
    buffer_write_ptr(client_data->write_buffer, &available_space);
    if (available_space >=  REQUEST_CONNECT_ANSWER) {
        int read_amount = recv(key->fd, aux_buff, READ_AMOUNT, MSG_DONTWAIT);
        bool finished = feed_sock_request_parser((struct sock_request_message *) (client_data->current_parser.request_message), aux_buff,
                                                 read_amount);
        if(!finished)
            return AUTHENTICATED;
        else {
            process_request_message(client_data->current_parser.request_message, key);
            return READY_TO_CONNECT;
        }
    }
    return AUTHENTICATED;

}
