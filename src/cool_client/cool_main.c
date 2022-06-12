#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "../parsing/cool_client_parsing/simpleResponseParser.h"
#include "../parsing/cool_client_parsing/generalResponseParser.h"

#define COOL_PORT 42069
#define RECV_BUFFER_SIZE 512
#define CREDS_LEN 128
#define PARAMS_LEN 64

#define QUERY 0xD0
#define MODIFY 0xBE

#define ADD_USER 0
#define REMOVE_USER 1
#define ENABLE_SPOOFING 2
#define DISABLE_SPOOFING 3

#define TOTAL_CONNECTIONS 0
#define CURRENT_CONNECTIONS 1
#define MAX_CURRENT_CONNECTIONS 2
#define TOTAL_BYTES_SENT 3
#define TOTAL_BYTES_RECV 4
#define CONNECTED_USERS 5

static int ask_method_and_parameters(uint8_t * action, uint8_t * method, uint8_t * parameters);
static int ask_parameters(uint8_t method, uint8_t * parameters);
static int ask_credentials(uint8_t * username, uint8_t * password);
static int ask_username(uint8_t * username);
static int ask_password(uint8_t * password);
static int ask_protocol(uint8_t * protocol);
static int send_credentials(int socket_fd, uint8_t * username, uint8_t * password);
static int send_method_and_parameters(int sock_fd, uint8_t action, uint8_t method, uint8_t param_len, uint8_t * parameters);
static int send_array(uint8_t socket_fd, uint8_t len, uint8_t * array);
static void print_status(uint16_t status);
static void print_response(uint8_t action, uint8_t method, uint8_t response_length, char * response);

int main(){
    uint8_t username[CREDS_LEN], password[CREDS_LEN];
    uint8_t action, method;
    uint8_t parameters[PARAMS_LEN];
    uint8_t  buff_recv[RECV_BUFFER_SIZE];

    int sock_fd = socket(AF_INET , SOCK_STREAM , 0);

    if(sock_fd < 0)
        return 1;

    struct sockaddr_in server_address_4;
    memset(&server_address_4, 0, sizeof(server_address_4));
    server_address_4.sin_family = AF_INET;
    server_address_4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address_4.sin_port = htons(COOL_PORT);

    if(connect(sock_fd, (struct sockaddr *) &server_address_4, sizeof(server_address_4)) < 0)
        return 1;

    while(1){

    printf("\n~~~~~~~~~~~Authentication~~~~~~~~~~~\n");

    ask_credentials(username, password);

    send_credentials(sock_fd, username, password);

    struct simple_response_message * simple_response = init_simple_response_parser();

    int read_amount;
    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_simple_response_parser(simple_response, (char *) buff_recv, read_amount));

    uint16_t returned_status = simple_response->status[0] << 8;
    returned_status += simple_response->status[1];
    
    print_status(returned_status);


    printf("\n~~~~~~~~~~~Action & Method~~~~~~~~~~~\n");

    int param_len = ask_method_and_parameters(&action, &method, parameters);

    send_method_and_parameters(sock_fd, action, method, param_len, parameters);

    close_simple_response_parser(simple_response);

    struct general_response_message * general_response = init_general_response_parser();

    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_general_response_parser(general_response, (char *) buff_recv, read_amount));

    print_response(general_response->action, general_response->method, general_response->response_length, general_response->response);

    close_general_response_parser(general_response);

    printf("\n=================================================\n\n");
    printf("\n=================================================\n\n");

    }

    return 0;
}

static int ask_method_and_parameters(uint8_t * action, uint8_t * method, uint8_t * parameters){
    uint8_t action_str[4];
    uint8_t method_str[4];
    uint8_t len;

    printf("Action: ");
    if(fgets((char *) action_str, CREDS_LEN, stdin) == 0)
        return -1;
    *action = strtol((char *) action_str, NULL, 0);

    printf("Method: ");
    if(fgets((char *) method_str, CREDS_LEN, stdin) == 0)
        return -1;
    *method = strtol((char *) method_str, NULL, 0);

    if(*action == QUERY)
        return 0;

    if( (len = ask_parameters(*method, parameters)) < 0)
        return -1;
    
    return len;
}

//Returns the number of bytes in the parameters and -1 in case of error
static int ask_parameters(uint8_t method, uint8_t * parameters){
    int ulen, plen;
    switch(method){
        case ADD_USER:
            ulen = ask_username(parameters+1);
            if(ulen < 0)
                return -1;
            parameters[0] = ulen;
            parameters += ulen+1;

            plen = ask_password(parameters+1);
            if(plen < 0)
                return -1;
            parameters[0] = plen;
            return ulen+plen+2;

        case REMOVE_USER:
            ulen = ask_username(parameters+1);
            if(ulen < 0)
                return -1;
            parameters[0] = ulen;
            return ulen+1;

        case ENABLE_SPOOFING:
            if(ask_protocol(parameters) < 0)
                return -1;
            return 1;

        case DISABLE_SPOOFING:
            if(ask_protocol(parameters) < 0)
                return -1;
            return 1;
        }
    return -1;
}

static int ask_credentials(uint8_t * username, uint8_t * password){
    if(ask_username(username) < 0)
        return -1;

    if(ask_password(password) < 0)
        return -1;

    return 0;
}

static int ask_username(uint8_t * username){
    printf("Username: ");
    fflush(stdout);
    if(fgets((char *) username, CREDS_LEN, stdin) == 0)
        return -1;
    return strlen((char *) username)-1;
}

static int ask_password(uint8_t * password){
    printf("Password: ");
    fflush(stdout);
    if(fgets((char *) password, CREDS_LEN, stdin) == 0)
        return -1;
    return strlen((char *) password)-1;
}

static int ask_protocol(uint8_t * protocol){
    uint8_t protocol_str[4];
    printf("Protocol: ");
    fflush(stdout);
    if(fgets((char *) protocol_str, CREDS_LEN, stdin) == 0)
        return -1;
    *protocol = strtol((char *) protocol_str, NULL, 0);
    return 0;
}

static int send_credentials(int socket_fd, uint8_t * username, uint8_t * password){
    uint8_t ulen = strlen((char *) username)-1;
    uint8_t plen = strlen((char *) password)-1;
    uint8_t version = 1;

    if(send(socket_fd, &version, 1, 0) <= 0)
        return -1;

    if(send(socket_fd, &ulen, 1, 0) <= 0)
        return -1;
    
    if(send_array(socket_fd, ulen, username) < 0)
        return -1;
        
    if(send(socket_fd, &plen, 1, 0) <= 0)
        return -1;

    if(send_array(socket_fd, plen, password) < 0)
        return -1;

    return 0;
}

static int send_method_and_parameters(int sock_fd, uint8_t action, uint8_t method, uint8_t param_len, uint8_t * parameters){
    if(send(sock_fd, &action, 1, 0) < 0)
        return -1;

    if(send(sock_fd, &method, 1, 0) < 0)
        return -1;

    if(send_array(sock_fd, param_len, parameters) < 0)
        return -1;

    return 0;
}

static int send_array(uint8_t socket_fd, uint8_t len, uint8_t * array){
    int ret;
    while(len > 0){
        if((ret = send(socket_fd, array, len, 0)) <= 0)
            return -1;
        len -= ret;
        array += ret;
    } 
    return 0;
}

static void print_status(uint16_t status){
    printf("Status: %X\n", status);
}

static void print_response(uint8_t action, uint8_t method, uint8_t response_length, char * response){
    printf("Action: %X\n", action);
    printf("Method: %X\n", method);
    *(response + response_length) = 0;
    printf("Response: %s\n", response);
}