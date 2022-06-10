#include "client_request_processor.h"

struct user_info{
    char username[30];
    char password[30];
};

struct user_info users[10]={
        {.username="shadad", .password="shadad"},
        {.username="mdedeu", .password="mdedeu"},
        {.username="scastagnino", .password="scastagnino"},
        {.username="gbeade", .password="gbeade"},
};

bool process_hello_message(struct sock_hello_message * data, struct selector_key * key){
    if(data==NULL || key == NULL || key->data ==NULL )
        return false;
    if(data->version != CURRENT_SOCK_VERSION )
        return false;

    bool accepted_method_given = false;

    sock_client * client_data = (sock_client * ) key->data;

    if(client_data->write_buffer == NULL)
        return false;

    size_t  available_space;
    buffer_write_ptr(client_data->write_buffer,&available_space);
    if(available_space < HELLO_ANSWER_LENGTH)
        return false;

    for(int i = 0; i < data->nmethods; i++){
        if(data->methods[i] == USERNAME_AUTHENTICATION)
            accepted_method_given = true;
    }

    buffer_write(client_data->write_buffer, CURRENT_SOCK_VERSION);

    if(accepted_method_given)
        buffer_write(client_data->write_buffer, USERNAME_AUTHENTICATION);
    else
        buffer_write(client_data->write_buffer, NON_METHODS_ACCEPTED);
    return accepted_method_given; 
}

bool process_authentication_message(struct sock_authentication_message * data, struct selector_key * key){
    if(data==NULL || key == NULL || key->data ==NULL ||
            data->version != SUBNEGOTIATION_CURRENT_VERSION || data->username_length == 0
            || data->password_length== 0 || data->username == NULL || data->password == NULL)
        return false;
    sock_client * client_data = (sock_client * ) key->data;
    if(client_data->write_buffer == NULL)
        return false;

    size_t  available_space;
    buffer_write_ptr(client_data->write_buffer,&available_space);
    if(available_space < AUTHENTICATION_ANSWER_LENGTH)
        return false;

    bool valid_user = false;
    for(int i = 0; users != NULL && i < N(users) ; i++){
        if(strcmp(data->username, users[i].username)==0 && 0==strcmp(data->password, users[i].password))
            valid_user = true;
    }

    buffer_write(client_data->write_buffer, CURRENT_SOCK_VERSION);
    if(valid_user)
        buffer_write(client_data->write_buffer, VALID_USER);
    else
        buffer_write(client_data->write_buffer, NO_VALID_USER);
    return valid_user; 

}

void process_request_message(struct sock_request_message * data, struct selector_key * key){
    if(data->cmd != CONNECT_COMMAND) //invalid command
        return ;

    sock_client * client_information = (sock_client *) key->data;
    if(data->atyp == IPV4ADDRESS) {
        struct sockaddr_in * addr = malloc(sizeof(struct sockaddr_in)) ;

        addr->sin_addr.s_addr = *((uint32_t *) data->ipv4);
        addr->sin_family = AF_INET;
        addr->sin_port = data->port[0];
        addr->sin_port <<= 8;
        addr->sin_port += data->port[1];
        addr->sin_port = htons(addr->sin_port);

        client_information->origin_address = ((struct sockaddr_storage*) addr);
        client_information -> origin_address_length = IPV4SIZE ;

    }else if(data->atyp == IPV6ADDRESS){
        struct sockaddr_in6 * addr = malloc(sizeof(struct sockaddr_in6));

        memcpy(addr->sin6_addr.__in6_u.__u6_addr8, data->ipv6, IPV6SIZE);
        addr->sin6_family = AF_INET6;
        addr->sin6_port = data->port[0];
        addr->sin6_port <<= 8;
        addr->sin6_port += data->port[1];
        addr->sin6_port = htons(addr->sin6_port);

        client_information->origin_address = ((struct sockaddr_storage*) addr);
        client_information-> origin_address_length = IPV6SIZE ;
    }

}

void generate_request_answer(struct sock_request_message * data, struct selector_key * key){
    sock_client * client_information = (sock_client *) key->data;
    buffer * answer_buffer = client_information->write_buffer;

    buffer_reset(answer_buffer);
    buffer_write(answer_buffer, CURRENT_SOCK_VERSION);

    if(client_information->origin_fd < 0)
        buffer_write(answer_buffer, HOST_UNREACHABLE);
    else
        buffer_write(answer_buffer, SUCCEEDED);

    buffer_write(answer_buffer, 0);

    //buffer_write(answer_buffer,data->atyp);
    buffer_write(answer_buffer, 1); //IPV4 atyp

    struct sockaddr address_info;
    socklen_t socklen = sizeof(struct sockaddr);
    getsockname(client_information->origin_fd, &address_info, &socklen);

    size_t available_space;
    uint8_t * writing_direction;
    writing_direction = buffer_write_ptr(answer_buffer, &available_space);

//  if(data->atyp == IPV4ADDRESS){
        struct sockaddr_in client_info_ipv4 = *((struct sockaddr_in *) &address_info);
        memcpy(writing_direction, &client_info_ipv4.sin_addr.s_addr, IPV4SIZE);
        buffer_write_adv(answer_buffer, IPV4SIZE);

        uint8_t significant = (client_info_ipv4.sin_port >> 8);
        buffer_write(answer_buffer, significant);

        uint8_t insignificant = (client_info_ipv4.sin_port & 255);
        buffer_write(answer_buffer, insignificant);

//    }else{
//        struct sockaddr_in6 client_info_ipv6 = *((struct sockaddr_in6 *) &address_info);
//        memcpy(writing_direction, client_info_ipv6.sin6_addr.__in6_u.__u6_addr8, IPV6SIZE);
//        buffer_write_adv(answer_buffer, IPV6SIZE);

//        uint8_t significant = (client_info_ipv6.sin6_port >> 8);
//        buffer_write(answer_buffer, significant);

//        uint8_t insignificant = (client_info_ipv6.sin6_port & 255);
//        buffer_write(answer_buffer, insignificant);
//    }
}



