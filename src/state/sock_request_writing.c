#include "sock_request_writing.h"

void sock_request_writing_arrival(unsigned state, struct selector_key * key){
    if(key==NULL || key->data == NULL )
        return;
    struct sock_client * client_information = (struct sock_client *) key->data;
    struct sock_request_message *  message = (struct sock_request_message * )client_information->parsed_message;
    if(message->connection_result != 0 ) //if not connected should have gone to sock_negative_request_writing
        return;
    generate_positive_request_answer(message, key);
    selector_set_interest(key->s, client_information->client_fd, OP_WRITE);
}

unsigned sock_request_writing_write_handler(struct selector_key * key){
    if(key==NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;

    if (client_data->write_buffer == NULL || !buffer_can_read(client_data->write_buffer))
        return CLOSING_CONNECTION;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    if(written_bytes < 0)
        return CLOSING_CONNECTION;

    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < write_amount)
        return SOCK_REQUEST_WRITING;
    else{
        char buff[32];
        struct sock_request_message * message = (struct sock_request_message *  ) client_data->parsed_message;
        sockaddr_to_human(buff,32,(struct sockaddr *) client_data->client_information);
        printf("Client : %s\t",buff);
        sockaddr_to_human(buff,32,(struct sockaddr * )client_data->origin_address);
        printf("Origin:%s\t",buff);
        printf("status=%d\n",message->connection_result);
        return CONNECTED;
    }

}


void sock_request_writing_departure(unsigned state, struct selector_key * key){
    if(key==NULL || key->data == NULL )
        return;
    struct sock_client * client_information = (struct sock_client *) key->data;
    close_sock_request_message((struct sock_request_message * )client_information->parsed_message);
    selector_set_interest(key->s, client_information->client_fd, OP_NOOP);
    selector_set_interest(key->s, client_information->origin_fd, OP_NOOP);
}