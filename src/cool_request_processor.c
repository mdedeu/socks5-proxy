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

static void write_error_response(buffer * buff, char * error_response, uint32_t * header);

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
    uint32_t header = 0;

    if(client_data->write_buffer == NULL)
        return;

    size_t len = sizeof(header);
    uint8_t * header_write = buffer_write_ptr(client_data->write_buffer, &len);
    buffer_write_adv(client_data->write_buffer, len);
    header += data->action << 24;
    header += data->method << 16;

    switch(data->action){
        case MODIFY:
            switch(data->method){
            case ADD_USER:
                if(data->username == NULL || data->password == NULL)
                    return;
                buffer_write(client_data->write_buffer, add_user_handler(data->ulen, data->username, data->plen, data->password));
                header += 0x0001;
                break;
            case REMOVE_USER:
                if(data->username == NULL)
                    return;
                buffer_write(client_data->write_buffer, remove_user_handler(data->ulen, data->username));
                header += 0x0001;
                break;
            case ENABLE_SPOOFING:
                buffer_write(client_data->write_buffer, enable_spoofing_handler(data->protocol));
                header += 0x0001;
                break;
            case DISABLE_SPOOFING:
                buffer_write(client_data->write_buffer, disable_spoofing_handler(data->protocol));
                header += 0x0001;
                break;
            case CHANGE_BUFFER_SIZE:
                buffer_write(client_data->write_buffer, change_buffer_size_handler(data->protocol));
                header += 0x0001;
                break;
            default:
                error = 1;
                break;
            }
            if(error){
                write_error_response(client_data->write_buffer, "Invalid modifier", &header);
            }
            break;
        case QUERY:
            switch(data->method){
            case GET_TOTAL_CONNECTIONS:
                result = get_total_connections();
                header += 0x0001;
                break;
            case GET_CURRENT_CONNECTIONS:
                result = get_current_connections();
                header += 0x0001;
                break;
            case GET_MAX_CURRENT_CONNECTIONS:
                result = get_max_current_connections();
                header += 0x0001;
                break;
            case GET_TOTAL_BYTES_SENT:
                result = get_total_bytes_sent();
                header += 0x0001;
                break;
            case GET_TOTAL_BYTES_RECV:
                result = get_total_bytes_recv();
                header += 0x0001;
                break;
            case GET_CONNECTED_USERS:
                result = get_connected_users();
                header += 0x0001;
                break;
            case GET_MAX_BUFFER_SIZE:
                result = get_max_buffer_size();
                header += 0x0001;
                break;
            case GET_USER_LIST:
                result = get_user_list();
                header += 0x0001;
                break;
            default:
                error = 1;
                break;
            }
            if(error){
                write_error_response(client_data->write_buffer, "Invalid query", &header);
            }
            else
                buffer_write(client_data->write_buffer, (uint8_t) (result & 255));
            break;
        default:
            write_error_response(client_data->write_buffer, "Invalid action", &header);
            break;
    }
    memcpy(header_write, &header, sizeof(header));
}


static void write_error_response(buffer * buff, char * error_response, uint32_t * header){
    *header |= 0xFFFF0000;
    *header += strlen(error_response);
    buffer_compact(buff);
    size_t len = strlen(error_response);
    uint8_t * response_write = buffer_write_ptr(buff, &len);
    memcpy(response_write, error_response, len);
    buffer_write_adv(buff, len);
}