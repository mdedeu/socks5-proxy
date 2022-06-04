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
    else         //todo: close the connection as the rfc said.
        buffer_write(client_data->write_buffer, NO_VALID_USER);
}

void process_request_message(struct sock_request_message *  data, struct selector_key * key){
    if(data->cmd != CONNECT_COMMAND) //invalid command
        return ;

    sock_client * client_information = (sock_client * ) key ;
//    client_information->origin_port = (uint16_t*)data->port ;
    if(data->atyp == IPV4ADDRESS) {
        struct sockaddr_in * addr = malloc(sizeof (struct sockaddr_in)) ;

        addr->sin_addr.s_addr = *((uint64_t *) data->ipv4);
        addr->sin_family = AF_INET;
        addr->sin_port = (uint16_t)*data->port;

        client_information->origin_address = *((struct sockaddr_storage*) addr);

        client_information -> origin_address_length = IPV4SIZE ;
    }else if (data->atyp == IPV6ADDRESS){
        struct sockaddr_in6 * addr = malloc(sizeof (struct sockaddr_in6));


        memcpy(addr->sin6_addr.__in6_u.__u6_addr8 ,data->ipv6,IPV6SIZE);
        addr->sin6_family = AF_INET6;
        addr->sin6_port =(uint16_t )* data->port;

        client_information->origin_address = *((struct sockaddr_storage*) addr);

        client_information-> origin_address_length = IPV6SIZE ;
    }
    else if(data->atyp == DOMAIN_NAME){
        struct selector_key  * thread_copy = malloc(sizeof (struct selector_key));
        memcpy(thread_copy,key,sizeof(struct selector_key));
        //validate
        pthread_t tid ;
        pthread_create(&tid,0,request_resolving_blocking,thread_copy);
    }
}


