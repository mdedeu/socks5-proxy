#include "metrics.h"

 struct metrics volatile_metrics = {
        .historical_total_connections = 0,
        .current_connections = 0 ,
        .bytes_received = 0 ,
        .bytes_sent = 0 ,
        .admin_users={
                {.username="shadad",.password="shadad"},
                {.username="mdedeu",.password="mdedeu"},
                {.username="gbeade",.password="gbeade"},
                {.username="scastagnino",.password="scastagnino"},
        },
        .password_dissector_enable = true,
        .registered_clients = 0,
        .max_current_connections=0,
        .clients_need_authentication = true,

};

bool add_user_handler(uint8_t ulen, uint8_t * username, uint8_t plen, uint8_t * password){
    bool already_included = false;
    for(int i = 0 ; i < volatile_metrics.registered_clients ; i++){
        if( strcmp(volatile_metrics.client_users[i]->username,(char*)username) == 0){
            already_included = true;
            break;
        }
    }
    if(!already_included){
        struct server_user_info * adding_user_info = calloc(1, sizeof(struct server_user_info));
        if(adding_user_info == NULL)
            return false;
        adding_user_info->username = malloc(ulen+1);
        if(adding_user_info->username == NULL )
            return false;
        memcpy(adding_user_info->username,(char*)username,ulen);
        adding_user_info->username[ulen] = 0;
        adding_user_info->password = malloc(plen+1);
        if(adding_user_info->password == NULL )
            return false;
        memcpy(adding_user_info->password,password, plen);
        adding_user_info->password[plen] = 0;
        adding_user_info->connected = false;
        volatile_metrics.client_users[volatile_metrics.registered_clients++] = adding_user_info;
    }

    return !already_included; //exit if it was not on the list
}

bool remove_all_users(){
    uint8_t total_clients = volatile_metrics.registered_clients;
    for(int i = 0; i < total_clients; i++){
        if(!remove_user_handler(strlen(volatile_metrics.client_users[0]->username),(uint8_t *) volatile_metrics.client_users[0]->username))
            return false;
    }
    return true;
}

bool remove_user_handler(uint8_t ulen, uint8_t * username){
    for(int i = 0 ; i < volatile_metrics.registered_clients ; i++){
        if(strcmp(volatile_metrics.client_users[i]->username,(char*)username) == 0){
            if(volatile_metrics.client_users[i]->username)
                free(volatile_metrics.client_users[i]->username);
            if(volatile_metrics.client_users[i]->password)
                free(volatile_metrics.client_users[i]->password);
            if(volatile_metrics.client_users[i])
                free(volatile_metrics.client_users[i]);
            if(volatile_metrics.registered_clients-- >= 0)
                volatile_metrics.client_users[i] = volatile_metrics.client_users[volatile_metrics.registered_clients];
            return true;
        }
    }
    return false;
}

bool enable_spoofing_handler(/*uint8_t protocol*/){
    if(!volatile_metrics.password_dissector_enable) {
        volatile_metrics.password_dissector_enable = true;
        return  true;
    }
    return false;
}

bool disable_spoofing_handler(/*uint8_t protocol*/){
    if(volatile_metrics.password_dissector_enable){
        volatile_metrics.password_dissector_enable =false ;
        return true;
    }
    return false;
}

void increment_current_connections(){
    volatile_metrics.historical_total_connections++;
    volatile_metrics.current_connections++ ;
    if(volatile_metrics.current_connections > volatile_metrics.max_current_connections)
        volatile_metrics.max_current_connections = volatile_metrics.current_connections;
}

void decrement_current_connections(){
    if(volatile_metrics.current_connections > 0 )
        volatile_metrics.current_connections--;
}

void increment_data_sent(uint64_t data_quantity){
    if(data_quantity < 0) return;
    volatile_metrics.bytes_sent += data_quantity ;
}


void increment_data_received(uint64_t data_quantity){
    if(data_quantity < 0) return;
    volatile_metrics.bytes_received += data_quantity ;
}

uint8_t change_buffer_size_handler(uint16_t size){
    if(size)
        volatile_metrics.clients_need_authentication = true;
    else
        volatile_metrics.clients_need_authentication = false;
    return 0;
}


uint64_t get_total_connections(){
    return volatile_metrics.historical_total_connections;
}

uint64_t get_current_connections(){
    return volatile_metrics.current_connections;
}

uint64_t get_max_current_connections(){
    return volatile_metrics.max_current_connections;
}

uint64_t get_total_bytes_sent(){
    return volatile_metrics.bytes_sent;
}

uint64_t get_total_bytes_recv(){
    return volatile_metrics.bytes_received;
}

struct server_user_info ** get_user_list(){
    return volatile_metrics.client_users;
}

uint8_t get_registered_clients(){
    return volatile_metrics.registered_clients;
}

uint64_t get_connected_users(){
    uint8_t connected_users = 0 ;
    for(int i = 0 ; i < volatile_metrics.registered_clients ; i++){
        if( volatile_metrics.client_users[i]->connected )
            connected_users ++ ;
    }
    return connected_users;
}

bool connect_user(char * username , char * password){
    for(int i = 0; i < volatile_metrics.registered_clients ; i++){
        if(strcmp(volatile_metrics.client_users[i]->username, username)==0 &&
        0==strcmp(volatile_metrics.client_users[i]->password, password)){
            if(volatile_metrics.client_users[i]->connected == false)
            volatile_metrics.client_users[i]->connected=true;
            return true;
        }
    }
    return false;
}

bool clients_need_authentication(){
    return volatile_metrics.clients_need_authentication;
}

bool set_clients_need_authentication(bool boolean){
    volatile_metrics.clients_need_authentication = boolean;
    return true;
}

void disconnect(char * username){
    for(int i = 0 ; i < volatile_metrics.registered_clients ; i++){
        if( strcmp(volatile_metrics.client_users[i]->username,(char*)username) == 0){
            if(volatile_metrics.client_users[i]->connected == true)
            volatile_metrics.client_users[i]->connected=false;
        }
    }
}