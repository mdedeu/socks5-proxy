#include "client_request_processor.h"

struct user_info{
    char username[30];
    char password[30];
};

struct user_info users[10]={
        {.username="shadad",.password="shadad"},
        {.username="mdedeu",.password="mdedeu"},
        {.username="scastagnino",.password="scastagnino"},
        {.username="gbeade",.password="gbeade"},
};


void process_hello_message(struct sock_hello_message * data, struct selector_key * key){
    //check the version
    bool accepted_method_given = false;
    for(int i = 0 ; i < data->nmethods ; i++){
            if(data->methods[i] == USERNAME_AUTHENTICATION)
                accepted_method_given = true;
    }
    sock_client * client_data = (sock_client * ) key->data;
    buffer_write(client_data->write_buffer,VERSION);
    if(accepted_method_given)
        buffer_write(client_data->write_buffer, USERNAME_AUTHENTICATION);
    else{
        buffer_write(client_data->write_buffer, NON_METHODS_ACCEPTED);
        //todo: close the connection as the rfc said.
    }
}

void process_authentication_message(struct sock_authentication_message * data, struct selector_key * key){
    bool valid_user = false;
    for(int i = 0 ; i <10 ; i++){
            if(strcmp(data->username,users[i].username) && strcmp(data->password,users[i].password))
                valid_user= true;
        }
    sock_client * client_data = (sock_client * ) key->data;
    buffer_write(client_data->write_buffer,VERSION);
    if(valid_user)
        buffer_write(client_data->write_buffer, VALID_USER);
    else
        buffer_write(client_data->write_buffer, NO_VALID_USER);





}

void process_request_message(struct sock_request_message *  data, struct selector_key * key);


