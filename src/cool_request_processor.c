#include "cool_request_processor.h"

struct user_info{
    char username[30];
    char password[30];
};

static struct user_info users[10]={
    {.username="shadad", .password="shadad"},
    {.username="mdedeu", .password="mdedeu"},
    {.username="scastagnino", .password="scastagnino"},
    {.username="gbeade", .password="gbeade"},
};

void process_cool_authentication_message(struct cool_protocol_authentication_message * data, struct selector_key * key){
    bool valid_user = false;
    for(int i = 0; i < 10; i++){
            if(strcmp(data->username, users[i].username) && strcmp(data->password, users[i].password))
                valid_user = true;
    }

    cool_client * client_data = (cool_client * ) key->data;
    buffer_write(client_data->write_buffer, VERSION);

    if(valid_user)
        buffer_write(client_data->write_buffer, VALID_USER);
    else         //todo: close the connection as the rfc said.
        buffer_write(client_data->write_buffer, NO_VALID_USER);
}

void process_cool_request_message(struct general_request_message * data, struct selector_key * key){

}
