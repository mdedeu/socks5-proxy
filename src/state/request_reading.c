#include "request_reading.h"
#define  MAX_READ_LENGTH 512

static bool try_connect( struct selector_key * key);
static bool try_connection_aux(struct selector_key * key, struct sock_request_message * client_message, int domain , socklen_t address_length );
static bool  create_resolution_thread( struct selector_key *key);


void request_reading_arrival(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data!=NULL){
        sock_client *client_data = (sock_client *) key->data;
        client_data->using_parser = init_sock_request_parser();
        client_data->parsed_message = init_sock_request_message();
        client_data->client_state = REQUESTING;
        selector_set_interest_key(key,OP_READ);
    }
}


unsigned request_reading_read_handler(struct selector_key *key){
    if(key==NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;

    char temp_buffer[MAX_READ_LENGTH];

    int received_amount = recv(key->fd, temp_buffer, MAX_READ_LENGTH, MSG_DONTWAIT);
    if(received_amount <= 0 )
        return CLOSING_CONNECTION;


    struct sock_request_message * received_message = (struct sock_request_message *)client_data->parsed_message;
    if(received_message == NULL)
        return CLOSING_CONNECTION;

    bool finished = feed_sock_request_parser(
            client_data->using_parser,
            received_message,
            temp_buffer,
            received_amount
    );

    if(!finished)
        return SOCK_REQUEST_READING;

    if(received_message->cmd != CONNECT_COMMAND){
        received_message->connection_result = status_command_not_supported;
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }

    if(received_message->atyp == DOMAIN_NAME){
        if(create_resolution_thread(key))
            return SOCK_REQUEST_READING;
        else{
            struct sock_request_message * message = (struct sock_request_message * )client_data->parsed_message;
            message->connection_result = status_general_socks_server_failure;
            return SOCK_NEGATIVE_REQUEST_WRITING;
        }
    }
    else if(received_message->atyp == IPV4ADDRESS || received_message->atyp == IPV6ADDRESS){
        if(try_connect(key))
            return ADDRESS_CONNECTING;
        else return SOCK_NEGATIVE_REQUEST_WRITING;
    }
    else return CLOSING_CONNECTION;

}


unsigned request_reading_block_handler(struct selector_key *key){
    if(key==NULL || key->data == NULL )
        return CLOSING_CONNECTION;
    sock_client *client_data = (sock_client *) key->data;
    if(client_data->origin_resolutions == NULL ){
        struct sock_request_message * message = (struct sock_request_message * )client_data->parsed_message;
        message->connection_result = status_general_socks_server_failure;
        return SOCK_NEGATIVE_REQUEST_WRITING;
    }
    return DOMAIN_CONNECTING;
}

void request_reading_departure(const unsigned state , struct selector_key * key ){
    if(key!=NULL && key->data !=NULL){
        sock_client *client_data = (sock_client *) key->data;
        close_sock_request_parser(client_data->using_parser);
        client_data->using_parser = NULL;
        selector_set_interest_key(key,OP_NOOP);
    }
}


static bool try_connect( struct selector_key * key){
    if(key == NULL || key->data == NULL )
        return false;
    sock_client * client_information  = (sock_client*) key->data;
    if(client_information->parsed_message == NULL)
        return false;
    struct sock_request_message * client_message = (struct sock_request_message *) client_information->parsed_message;

    process_request_message(client_message,key);

    if(client_information->origin_address_length == IPV4SIZE)
        return try_connection_aux( key, client_message, AF_INET , sizeof(struct sockaddr_in));
    else if(client_information->origin_address_length == IPV6SIZE)
        return try_connection_aux(key,client_message,AF_INET6, sizeof(struct sockaddr_in6));
    else return false;

}

static bool try_connection_aux(struct selector_key * key, struct sock_request_message * client_message, int domain , socklen_t address_length ){
    sock_client *client_information = (sock_client *) key->data;
    int  server_socket_fd = socket(domain, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket_fd < 0 ){
        client_message->connection_result= status_general_socks_server_failure;
        return false;
    }

    if(SELECTOR_SUCCESS != selector_fd_set_nio(server_socket_fd)){
        client_message->connection_result = status_general_socks_server_failure;
        return false;
    }

    client_information->origin_fd = server_socket_fd;
    if(client_information->origin_address == NULL )
        return false;
    connect(server_socket_fd, (struct sockaddr *) (client_information->origin_address), address_length);
    switch (errno) {
        case ECONNREFUSED:
            client_message->connection_result = status_connection_refused;
            return false;
        case EHOSTUNREACH:
            client_message->connection_result = status_host_unreachable;
            return false;
        case ENETUNREACH:
            client_message->connection_result = status_network_unreachable;
            return false;
        case EINPROGRESS:
            if(SELECTOR_SUCCESS == selector_register(key->s, server_socket_fd, &socks5_handler, OP_WRITE, client_information))
                return true;
        default:
            client_message->connection_result = status_general_socks_server_failure;
            return false;
    }
    
}


static bool  create_resolution_thread( struct selector_key *key){
    struct selector_key * thread_copy = malloc(sizeof(struct selector_key));
    if(thread_copy == NULL )
        return false;
    memcpy(thread_copy, key, sizeof(struct selector_key));
    pthread_t tid;
   if(0!= pthread_create(&tid, 0, &request_resolving_blocking, thread_copy))
       return false;
    set_as_blocked_by_server(key);
   return true;
}
