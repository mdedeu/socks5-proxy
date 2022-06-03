#include "client_request_processor.h"


void process_hello_message(struct sock_hello_message data, struct selector_key * key){
    //check the version
    bool accepted_method_given = false;
    for(int i = 0 ; i < data->nmethods ; i++){
            if(data.methods[i] == USERNAME_AUTHENTICATION)
                accepted_method_given = true;
    }
    sock_client * client_data = (sock_client * ) key->data;
    buffer_write(client_data->write_buffer,VERSION);
    if(accepted_method_given)
        buffer_write(client_data->write_buffer, USERNAME_AUTHENTICATION);
    else
        buffer_write(client_data->write_buffer, NON_METHODS_ACCEPTED);
}

void process_authentication_message(struct sock_authentication_message data, struct selector_key * key);

void process_request_message(struct sock_request_message data, struct selector_key * key);


