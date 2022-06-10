#include "cool_authenticate_reading.h"
#define COOL_AUTHENTICATION_MAX_LENGTH 2
#define  MAX_READ_LENGTH 512

 void cool_authenticate_reading_departure(const unsigned int leaving_state, struct selector_key *key){
     cool_client *client_data = (cool_client *) key->data;
     //TODO: implementar los procesadores de la data
     process_cool_authentication_message((struct cool_protocol_authentication_message * ) client_data->parsed_message, key);
     close_cool_protocol_authentication_parser(client_data->using_parser);
     close_cool_protocol_authentication_message((struct cool_protocol_authentication_message * ) client_data->parsed_message);
     selector_set_interest_key(key, OP_NOOP);

}


unsigned cool_authenticate_read_handler(struct selector_key *key){
    cool_client * client_data = (cool_client *) key->data;

    char temp_buffer[MAX_READ_LENGTH];

    int received_amount = recv(key->fd, temp_buffer, MAX_READ_LENGTH, MSG_DONTWAIT);

    bool finished = feed_cool_protocol_authentication_parser(
            client_data->using_parser,
        (struct cool_protocol_authentication_message *) (client_data->parsed_message),
            temp_buffer,
            received_amount
    );

    return finished ?  COOL_AUTHENTICATE_WRITING : COOL_AUTHENTICATE_READING;
}


void cool_authenticate_reading_arrival(const unsigned int leaving_state, struct selector_key *key){
    cool_client *client_data = (cool_client *) key->data;
    client_data->using_parser = init_cool_protocol_authentication_parser();
    client_data->parsed_message= init_cool_protocol_authentication_message();
    selector_set_interest_key(key,OP_READ);
}

