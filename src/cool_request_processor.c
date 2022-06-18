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

static void write_error_response(buffer * buff, uint8_t * header);

void process_cool_authentication_message(struct cool_protocol_authentication_message * data, struct selector_key * key){
    //TODO: hace la comprarcion con un metodo del sistema de metricas
    bool valid_user = false;
    if(data->username == NULL || data->password == NULL){
        valid_user = false;
        return;
    }

    for(int i = 0; i < 10; i++){
            if(!strcmp(data->username, users[i].username) && !strcmp(data->password, users[i].password))
                valid_user = true;
    }

    if(key == NULL || key->data == NULL)
        return;

    cool_client * client_data = (cool_client * ) key->data;
    
    if(client_data->write_buffer == NULL)
        return;

    if(valid_user){
        buffer_write(client_data->write_buffer, 0xC0);
        buffer_write(client_data->write_buffer, 0x01);
    }
    else{         //TODO: close the connection as the rfc said.
        buffer_write(client_data->write_buffer, 0x4B);
        buffer_write(client_data->write_buffer, 0x1D);
    }
}

//TODO: para ver como se implementan los metodos haca falta ver si se necesita un buffer auxiliar
void process_cool_request_message(struct general_request_message * data, struct selector_key * key){
    if(key == NULL || key->data == NULL)
        return;
    

    cool_client * client_data = (cool_client * ) key->data;
    uint64_t result;
    int error = 0;
    uint8_t header[4] = {0};
    struct server_user_info ** user_list;

    if(client_data->write_buffer == NULL)
        return;

    buffer_reset(client_data->write_buffer);

    size_t len = sizeof(header)/sizeof(header[0]);
    size_t available_write, number_len;
    uint8_t * header_write = buffer_write_ptr(client_data->write_buffer, &available_write);
    buffer_write_adv(client_data->write_buffer, len);
    header[0] = data->action;
    header[1] += data->method;

    switch(data->action){
        case MODIFY:
            switch(data->method){
            case ADD_USER:
                if(data->username == NULL || data->password == NULL)
                    return;
                if(!add_user_handler(data->ulen, data->username, data->plen, data->password)){
                    error = 1;
                    break;
                }
                buffer_write(client_data->write_buffer, 1);
                header[3] = 0x01;
                break;
            case REMOVE_USER:
                if(data->username == NULL)
                    return;
                if(!remove_user_handler(data->ulen, data->username)){
                    error = 1;
                    break;
                }
                buffer_write(client_data->write_buffer, 1);
                header[3] = 0x01;
                break;
            case ENABLE_SPOOFING:
                if(!enable_spoofing_handler(data->protocol)){
                    error = 1;
                    break;
                }
                buffer_write(client_data->write_buffer, 1);
                header[3] = 0x01;
                break;
            case DISABLE_SPOOFING:
                if(!disable_spoofing_handler(data->protocol)){
                    error = 1;
                    break;
                }
                buffer_write(client_data->write_buffer, 1);
                header[3] = 0x01;
                break;
            case CHANGE_BUFFER_SIZE:
                if(!set_clients_need_authentication(data->clients_need_authentication)){
                    error = 1;
                    break;
                }
                buffer_write(client_data->write_buffer, 1);
                header[3] = 0x01;
                break;
            default:
                error = 1;
                break;
            }
            if(error){
                write_error_response(client_data->write_buffer, header);
            }
            break;
        case QUERY:
            switch(data->method){
            case GET_TOTAL_CONNECTIONS:
                result = get_total_connections();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_CURRENT_CONNECTIONS:
                result = get_current_connections();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_MAX_CURRENT_CONNECTIONS:
                result = get_max_current_connections();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_TOTAL_BYTES_SENT:
                result = get_total_bytes_sent();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_TOTAL_BYTES_RECV:
                result = get_total_bytes_recv();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_CONNECTED_USERS:
                result = get_connected_users();
                header[3] = 0x08;
                number_len = sizeof(result)/sizeof(uint8_t);
                for(size_t i = 0; i < number_len; i++)
                    buffer_write(client_data->write_buffer, (uint8_t) (result >> (8*(number_len - i - 1))));
                break;
            case GET_USER_LIST:
                user_list = get_user_list();
                uint16_t response_len = 0;
                int clients_num = get_registered_clients();
                size_t username_len;
                for(int i = 0; i < clients_num; i++){
                    username_len = strlen(user_list[i]->username);
                    buffer_write(client_data->write_buffer, username_len);
                    size_t available_space;
                    uint8_t * write_from = buffer_write_ptr(client_data->write_buffer, &available_space);
                    memcpy(write_from, user_list[i]->username, username_len);
                    buffer_write_adv(client_data->write_buffer, username_len);
                    response_len += username_len + 1;
                }
                header[2] = (uint8_t) (response_len & (uint16_t) 0xFF00);
                header[3] = (uint8_t) (response_len & (uint16_t) 0x00FF);
                break;
            default:
                error = 1;
                break;
            }
            if(error){
                write_error_response(client_data->write_buffer, header);
            }
            break;
        default:
            write_error_response(client_data->write_buffer, header);
            break;
    }
    memcpy(header_write, header, sizeof(header));
}


static void write_error_response(buffer * buff, uint8_t * header){
    header[0] = 0xFF;
    header[1] = 0xFF;
    header[2] = 0x00;
    header[3] = 0x01;
    buffer_write(buff, 1);
}