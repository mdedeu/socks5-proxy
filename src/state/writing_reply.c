#include "writing_reply.h"

void writing_reply_on_arrival(unsigned state , struct selector_key * key ){
    struct sock_client  * client_information = (struct sock_client * )key->data;
    generate_request_answer(client_information->current_parser.request_message,key);
    selector_set_interest(key->s,client_information->client_fd,OP_WRITE);
}

unsigned  writing_reply_write_handler(struct selector_key * key){
    struct sock_client  * client_information = (struct sock_client * )key->data;

    if(!buffer_can_write(client_information->write_buffer))
        return WRITING_REPLY;

    size_t  available_write ;
    uint8_t  * writing_direction = buffer_write_ptr(client_information->write_buffer,&available_write);

    size_t available_read;
    uint8_t * reading_direction = buffer_read_ptr(client_information->current_parser.request_message->answer_buffer,&available_read);

    int minimum_space = available_read>=available_write?available_write:available_read;
    memcpy(writing_direction, reading_direction,minimum_space);
    buffer_write_adv(client_information->write_buffer,minimum_space);
    buffer_read_adv(client_information->current_parser.request_message->answer_buffer,minimum_space);

    reading_direction = buffer_read_ptr(client_information->write_buffer,&available_read);
    int sent_bytes= send(client_information->client_fd,reading_direction,available_read,MSG_DONTWAIT);
    buffer_read_adv(client_information->write_buffer,sent_bytes);

    if(!buffer_can_read(client_information->current_parser.request_message->answer_buffer) && !buffer_can_read(client_information->write_buffer)){

        return CONNECTED;
    }
    return WRITING_REPLY;


}