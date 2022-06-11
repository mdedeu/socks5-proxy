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

//TODO: para ver como se implementan los metodos haca falta ver si se necesita un buffer auxiliar
void process_cool_request_message(struct general_request_message * data, struct selector_key * key){
    cool_client * client_data = (cool_client * ) key->data;

    buffer_write(client_data->write_buffer, data->action);
    buffer_write(client_data->write_buffer, data->method);

    switch(data->action){
        case MODIFY:
            buffer_write(client_data->write_buffer, 1);
            switch(data->method){
            case ADD_USER:
                buffer_write(client_data->write_buffer, (data->ulen, data->username, data->plen, data->password));
                break;
            case REMOVE_USER:
                buffer_write(client_data->write_buffer, remove_user_handler(data->ulen, data->username));
                break;
            case ENABLE_SPOOFING:
                buffer_write(client_data->write_buffer, enable_spoofing_handler(data->protocol));
                break;
            case DISABLE_SPOOFING:
                buffer_write(client_data->write_buffer, disable_spoofing_handler(data->protocol));
                break;
            }
            break;
        case QUERY:
            uint64_t result;
            switch(data->method){
            case TOTAL_CONNECTIONS:
                result = total_connections_handler();
                break;
            case CURRENT_CONNECTIONS:
                result = current_connections_handler();
                break;
            case MAX_CURRENT_CONNECTIONS:
                result = max_current_connections_handler();
                break;
            case TOTAL_BYTES_SENT:
                result = total_bytes_sent_handler();
                break;
            case TOTAL_BYTES_RECV:
                result = total_bytes_recv_handler();
                break;
            case CONNECTED_USERS:
                result = connected_users_handler();
                break;
            }
            buffer_write(client_data->write_buffer, 8);
            for(int i = 0; i < 7; i++)
                buffer_write(client_data->write_buffer, (result >> i*8) & 255);
            break;
    }
}


static uint8_t add_user_handler(uint8_t ulen, char * username, uint8_t plen, char * password){
    return 255;
}

static uint8_t remove_user_handler(uint8_t ulen, char * username){
    return 255;
}

static uint8_t enable_spoofing_handler(uint8_t protocol){
    return 255;
}

static uint8_t disable_spoofing_handler(uint8_t protocol){
    return 255;

}

static uint64_t total_connections_handler(){
    return 255;
}

static uint64_t current_connections_handler(){
    return 255;
}

static uint64_t max_current_connections_handler(){
    return 255;
}

static uint64_t total_bytes_sent_handler(){
    return 255;
}

static uint64_t total_bytes_recv_handler(){
    return 255;
}

static uint64_t connected_users_handler(){
    return 255;
}