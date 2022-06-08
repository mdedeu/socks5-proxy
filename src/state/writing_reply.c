#include "writing_reply.h"

void writing_reply_on_arrival(unsigned state, struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;

    generate_request_answer(client_information->current_parser.request_message, key);
    selector_set_interest(key->s, client_information->client_fd, OP_WRITE);
}

unsigned writing_reply_write_handler(struct selector_key * key){
    struct sock_client * client_information = (struct sock_client *) key->data;
    size_t available_read;
    uint8_t * reading_direction;

    if(buffer_can_read(client_information->write_buffer)){
        reading_direction = buffer_read_ptr(client_information->write_buffer, &available_read);
        int sent_bytes = send(client_information->client_fd, reading_direction, available_read, MSG_DONTWAIT);

        buffer_read_adv(client_information->write_buffer, sent_bytes);
        buffer_compact(client_information->write_buffer);
    }

    if(buffer_can_write(client_information->write_buffer) && buffer_can_read(client_information->current_parser.request_message->answer_buffer)){
        size_t available_write;

        uint8_t * writing_direction = buffer_write_ptr(client_information->write_buffer, &available_write);
        reading_direction = buffer_read_ptr(client_information->current_parser.request_message->answer_buffer, &available_read);

        int minimum_space = available_read <= available_write ? available_read : available_write;
        memcpy(writing_direction, reading_direction, minimum_space);

        buffer_write_adv(client_information->write_buffer,minimum_space);
        buffer_read_adv(client_information->current_parser.request_message->answer_buffer, minimum_space);
        buffer_compact(client_information->current_parser.request_message->answer_buffer);
    }

    if(!buffer_can_read(client_information->current_parser.request_message->answer_buffer) && !buffer_can_read(client_information->write_buffer))
        return CONNECTED;

    return WRITING_REPLY;
}