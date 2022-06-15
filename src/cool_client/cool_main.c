#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
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
#define MAX_AUTH_TRIES 3
#define COMMAND_MAX_LEN 128

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
static void print_help();
static int close_connection(int socket_fd);
static void print_welcome();
static int resolve_command(char * command, uint8_t * method, uint8_t * action, uint8_t * parameters);

#define QUERIES_TOTAL 7
#define MODIFIERS_TOTAL 5
char * queries[] = {"gthc", "gcc", "gmcc", "gtbs", "gnuc", "gmbs", "gul"};
char * modifiers[] = {"au", "ru", "eps", "dps", "cbs"};

int main(int argc, char * argv[]){
    if(argc != 2)
        return -1;

    errno = 0;
    int address_family = strtol(argv[1], NULL, 10);

    if(errno != 0)
        return -1;
    if(address_family != 4 && address_family != 6)
        return -1;


    uint8_t username[CREDS_LEN] = {0}, password[CREDS_LEN] = {0};
    uint8_t action, method;
    uint8_t parameters[PARAMS_LEN] = {0};
    uint8_t  buff_recv[RECV_BUFFER_SIZE] = {0};
    uint16_t returned_status = 0;
    int read_amount;

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

    int tries=0;
    while(returned_status != 0xC001){

    ask_credentials(username, password);

    send_credentials(sock_fd, username, password);

    struct simple_response_message * simple_response = init_simple_response_parser();

    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_simple_response_parser(simple_response, (char *) buff_recv, read_amount));

    returned_status = simple_response->status[0] << 8;
    returned_status += simple_response->status[1];

    print_status(returned_status);

    close_simple_response_parser(simple_response);

    putchar('\n');

    if(++tries >= MAX_AUTH_TRIES){
        printf("Max number of tries reached, closing client\n.");
        return close_connection(sock_fd);
    }
    }

    while(1){

    int param_len = ask_method_and_parameters(&action, &method, parameters);

    send_method_and_parameters(sock_fd, action, method, param_len, parameters);

    struct general_response_message * general_response = init_general_response_parser();

    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_general_response_parser(general_response, (char *) buff_recv, read_amount));

    print_response(general_response->action, general_response->method, general_response->response_length, general_response->response);

    close_general_response_parser(general_response);

    }

    return 0;
}

static int ask_method_and_parameters(uint8_t * action, uint8_t * method, uint8_t * parameters){
    char command[COMMAND_MAX_LEN];
    printf("> ");

    fflush(stdout);
     if(fgets(command, COMMAND_MAX_LEN-1, stdin) == 0)
         return -1;
     command[strlen(command)-1]=0;

    if(resolve_command(command, action, method, parameters) == -1)
        return -1;
    
    if(*parameters)
        return ask_parameters(*method, parameters);

    return 0;
}

//Returns the number of bytes in the parameters and -1 in case of error
static int ask_parameters(uint8_t method, uint8_t * parameters){
    int ulen, plen;
    switch(method){
        case ADD_USER:
            ulen = ask_username(parameters+1);
            if(ulen < 0)
                return -1;
            parameters[0] = (uint8_t) ulen;
            parameters += ulen+1;

            plen = ask_password(parameters+1);
            if(plen < 0)
                return -1;
            parameters[0] = (uint8_t) plen;
            return ulen+plen+2;

        case REMOVE_USER:
            ulen = ask_username(parameters+1);
            if(ulen < 0)
                return -1;
            parameters[0] = (uint8_t) ulen;
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
    printf("Enter username: ");
    fflush(stdout);
    if(fgets((char *) username, CREDS_LEN, stdin) == 0)
        return -1;
    return strlen((char *) username)-1;
}

static int ask_password(uint8_t * password){
    printf("Enter password: ");
    fflush(stdout);
    if(fgets((char *) password, CREDS_LEN, stdin) == 0)
        return -1;
    return strlen((char *) password)-1;
}

static int ask_protocol(uint8_t * protocol){
    uint8_t protocol_str[6];
    printf("Protocol: ");
    fflush(stdout);
    if(fgets((char *) protocol_str, CREDS_LEN, stdin) == 0)
        return -1;
    protocol_str[4]=0;
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

    if(param_len > 0)
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
    if(status == 0xC001)
        print_welcome();
    else if(status == 0x4B1D)
        printf("Invalid credentials, please try again.\n");
    else{
        printf("Please enter valid credentials\n");
    }
}

static void print_response(uint8_t action, uint8_t method, uint8_t response_length, char * response){
    //printf("Action: %X\n", action);
    //printf("Method: %X\n", method);
    *(response + response_length) = 0;
    printf("\nResponse: %d\n", response[0]);
}

static int close_connection(int socket_fd){
    return close(socket_fd);
}

static void print_welcome(){
    printf("Welcome to the cool sock5 proxy configuration\nEnter the help command in case you need any help\n");
}

//If the command is invalid it returns -1, else 0
static int resolve_command(char * command, uint8_t * method, uint8_t * action, uint8_t * parameters){

    for(int i = 0; i < QUERIES_TOTAL; i++){
        if(!strcmp(command, queries[i])){
            *method = QUERY;
            *action = i+1;
            *parameters = 0;
            return 0;
        }
    }

    for(int i = 0; i < MODIFIERS_TOTAL; i++){
        if(!strcmp(command, modifiers[i])){
            *method = MODIFY;
            *action = i;
            *parameters = 1;
            return 0;
        }
    }

    return -1;
}