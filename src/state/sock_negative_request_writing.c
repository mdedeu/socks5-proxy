#include "sock_negative_request_writing.h"
#define WRITING_SIZE 10
#define REPLY_FIELD 1
static uint8_t writing_data[WRITING_SIZE]={5,0,0,1,0,0,0,0,0,0};



void sock_negative_request_writing_arrival(unsigned state, struct selector_key * key){
    if(key==NULL || key->data == NULL)
        return;
    struct sock_client * client_information = (struct sock_client *) key->data;
    struct sock_request_message * message = (struct sock_request_message *  ) client_information->parsed_message;
    writing_data[REPLY_FIELD] = message->connection_result;
    buffer * writing_buffer = client_information->write_buffer;
    if(writing_buffer == NULL)
        return;
    uint8_t * writing_direction;
    size_t buffer_space ;
    writing_direction = buffer_write_ptr(writing_buffer , & buffer_space ) ;
    if(buffer_space < WRITING_SIZE)
        return ;
    else {
        memcpy(writing_direction,&writing_data, WRITING_SIZE);
        buffer_write_adv(writing_buffer, WRITING_SIZE);
    }
    selector_set_interest(key->s,client_information->client_fd,OP_WRITE);
}

unsigned sock_negative_request_write_handler(struct selector_key * key){
    if(key == NULL || key->data == NULL)
        return CLOSING_CONNECTION;
    sock_client * client_data = (sock_client *) key->data;
    buffer * writing_buffer = client_data->write_buffer;
    if(writing_buffer == NULL || !buffer_can_read(writing_buffer))
        return CLOSING_CONNECTION;

    size_t write_amount;
    uint8_t *reading_since = buffer_read_ptr(client_data->write_buffer, &write_amount);
    ssize_t written_bytes = send(client_data->client_fd, reading_since, write_amount, MSG_DONTWAIT);
    if(written_bytes < 0 )
        return CLOSING_CONNECTION;

    buffer_read_adv(client_data->write_buffer, written_bytes);
    buffer_compact(client_data->write_buffer);

    if(written_bytes < write_amount) // check if not connected
        return SOCK_NEGATIVE_REQUEST_WRITING;
    else{
        print_connection_data(key);
        return CLOSING_CONNECTION;
    }
}

void sock_negative_request_writing_departure(unsigned state, struct selector_key * key) {
    if (key == NULL || key->data == NULL)
        return;
    struct sock_client *client_information = (struct sock_client *) key->data;
    close_sock_request_message((struct sock_request_message *) client_information->parsed_message);
    client_information->parsed_message = NULL;
    selector_set_interest(key->s, client_information->client_fd, OP_NOOP);
    selector_set_interest(key->s, client_information->client_fd, OP_NOOP);
}
