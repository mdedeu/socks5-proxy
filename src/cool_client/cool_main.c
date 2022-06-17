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
#include <netinet/in.h>
#include <netinet/ip.h>
#include <inttypes.h>
#include "../parsing/cool_client_parsing/simpleResponseParser.h"
#include "../parsing/cool_client_parsing/generalResponseParser.h"

#define COOL_PORT 42069
#define RECV_BUFFER_SIZE 512
#define CREDS_LEN 128
#define NUMERIC_INPUT_LEN 32
#define PARAMS_LEN 64
#define IPV6SIZE 16
#define IPV4SIZE 4

#define QUERY 0xD0
#define MODIFY 0xBE

#define ADD_USER 0
#define REMOVE_USER 1
#define ENABLE_SPOOFING 2
#define DISABLE_SPOOFING 3
#define ACTIVATE_AUTHENTICATION 4
#define DEACTIVATE_AUTHENTICATION 5

#define TOTAL_CONNECTIONS 1
#define CURRENT_CONNECTIONS 2
#define MAX_CURRENT_CONNECTIONS 3
#define TOTAL_BYTES_SENT 4
#define TOTAL_BYTES_RECV 5
#define CONNECTED_USERS 6
#define USER_LIST 7

#define MAX_AUTH_TRIES 3
#define COMMAND_MAX_LEN 64

static int ask_method_and_parameters(int sock_fd, int * is_builtin, uint8_t * action, uint8_t * method, uint8_t * parameters);
static int ask_parameters(uint8_t * method, uint8_t * parameters);
static int ask_credentials(uint8_t * username, uint8_t * password);
static int ask_username(uint8_t * username);
static int ask_password(uint8_t * password);
static int ask_protocol(uint8_t * protocol);
static int send_credentials(int socket_fd, uint8_t * username, uint8_t * password);
static int send_method_and_parameters(int sock_fd, uint8_t action, uint8_t method, uint8_t param_len, uint8_t * parameters);
static int send_array(uint8_t socket_fd, uint8_t len, uint8_t * array);
static void print_status(uint16_t status);
static void print_response(uint8_t action, uint8_t method, uint16_t response_length, unsigned char * response);
static void handle_help(int sock_fd);
static void handle_quit(int sock_fd);
static int close_connection(int socket_fd);
static void print_welcome();
static int resolve_command(char * command, uint8_t * method, uint8_t * action, uint8_t * parameters);
static int connect_to_ipv4(struct sockaddr_in * ipv4_address);
static int connect_to_ipv6(struct sockaddr_in6 * ipv6_address);

#define BUILTIN_TOTAL 2
#define QUERIES_TOTAL 7
#define MODIFIERS_TOTAL 6
char * builtin_names[] = {"help", "quit"};
void (*builtin[])(int) = {handle_help, handle_quit};

char * queries[] = {"gthc", "gcc", "gmcc", "gtbs", "gtbr", "gnuc", "gul"};
char * query_description[] = {
    "gthc - Get Total Historic Connections",
    "gcc - Get Current Connections",
    "gmcc - Get Max. Concurrent Connections",
    "gtbs - Get Total Bytes Sent",
    "gtbr - Get Total Bytes Received",
    "gnuc - Get Number of Users Connected",
    "gul - Get User List"};

char * modifiers[] = {"au", "ru", "eps", "dps", "aa", "da"};
char * modifier_description[] = {
    "au - Add User",
    "ru - Remove User",
    "eps - Enable Password Spoofing",
    "dps - Disable Password Spoofing",
    "aa - Activate Authentication",
    "da - Deactivate Authentication"};


int main(int argc, char * argv[]){

    uint8_t username[CREDS_LEN] = {0}, password[CREDS_LEN] = {0};
    uint8_t action, method;
    uint8_t parameters[PARAMS_LEN] = {0};
    uint8_t  buff_recv[RECV_BUFFER_SIZE] = {0};
    uint16_t returned_status = 0;
    int read_amount, is_builtin;
    struct sockaddr_in6 ipv6_address;
    struct sockaddr_in ipv4_address;

    if(argc != 2){
        printf("You should enter the address family as a parameter, 4 for ipv4 and 6 for ipv6.\n");
        return -1;
    }

    errno = 0;
    int address_family = strtol(argv[1], NULL, 10);

    if(errno != 0)
        return -1;

    if(address_family != 4 && address_family != 6){
        printf("You should enter the address family as a parameter, 4 for ipv4 and 6 for ipv6.\n");
        return -1;
    }

    int sock_fd;
    if(address_family == 4)
        sock_fd = connect_to_ipv4(&ipv4_address);
    else
        sock_fd = connect_to_ipv6(&ipv6_address);
    
    if(sock_fd < 0)
        return -1;

    int tries=0;
    while(returned_status != 0xC001){

    if(ask_credentials(username, password) < 0)
        continue;

    if(tries++ >= MAX_AUTH_TRIES){
        printf("Max number of tries reached\n");
        return close_connection(sock_fd);
    }

    if(send_credentials(sock_fd, username, password) < 0){
        close_connection(sock_fd);
        return -1;
    }

    struct simple_response_message * simple_response = init_simple_response_parser();

    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount <= 0){
            close_connection(sock_fd);
            return -1;
        }
    } while(!feed_simple_response_parser(simple_response, (unsigned char *) buff_recv, read_amount));

    returned_status = simple_response->status[0] << 8;
    returned_status += simple_response->status[1];

    print_status(returned_status);

    close_simple_response_parser(simple_response);

    putchar('\n');
    }

    while(1){

    int param_len = ask_method_and_parameters(sock_fd, &is_builtin, &action, &method, parameters);
    if(param_len < 0){
        close_connection(sock_fd);
        return -1;
    }
    if(is_builtin)
        continue;

    if(send_method_and_parameters(sock_fd, action, method, param_len, parameters) < 0){
        close_connection(sock_fd);
        return -1;
    }

    struct general_response_message * general_response = init_general_response_parser();

    do {
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount <= 0){
            close_connection(sock_fd);
            return -1;
        }
    } while(!feed_general_response_parser(general_response, (unsigned char *) buff_recv, read_amount));

    print_response(general_response->action, general_response->method, general_response->response_length, general_response->response);

    close_general_response_parser(general_response);

    }

    return 0;
}

static int ask_method_and_parameters(int sock_fd, int * skip, uint8_t * action, uint8_t * method, uint8_t * parameters){
    char command[COMMAND_MAX_LEN];
    printf("> ");
    fflush(stdout);
    int len;

    if(!fgets(command, COMMAND_MAX_LEN, stdin)){
        return -1;
    }

    char * end = strchr(command, '\n');
    if(end == NULL){
        printf("Invalid command\n");
        while(getc(stdin) != '\n');
        *skip= 1;
        return 0;
    }
    else
        command[end-command] = 0; 


    for(int i = 0; i < BUILTIN_TOTAL; i++){
        if(!strcmp(builtin_names[i], command)){
            *skip= 1;
            builtin[i](sock_fd);
            return 0;
        }
    }

    if(resolve_command(command, action, method, parameters) == -1){
        printf("Invalid command\n");
        *skip = 1;
        return 0;
    }

    
    if(*parameters){
        len = ask_parameters(method, parameters);
        if(len < 0){
            printf("Invalid parameter\n");
            *skip= 1;
            return 0;
        }
        *skip = 0;
        return len;
    }

    *skip= 0;

    return 0;
}

//Returns the number of bytes in the parameters and -1 in case of error
static int ask_parameters(uint8_t * method, uint8_t * parameters){
    int ulen, plen;
    switch(*method){
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

        case ACTIVATE_AUTHENTICATION:
            parameters[0] = 1;
            return 1;

        case DEACTIVATE_AUTHENTICATION:
            *method = 4;
            parameters[0] = 0;
            return 1;

        }
    return -1;
}

static int ask_credentials(uint8_t * username, uint8_t * password){
    if(ask_username(username) < 0){
        printf("Please enter credentials with valid format\n");
        return -1;
    }

    if(ask_password(password) < 0){
        printf("Please enter credentials with valid format\n");
        return -1;
    }

    return 0;
}

static int ask_username(uint8_t * username){
    printf("Enter username: ");
    fflush(stdout);
    if(!fgets((char *) username, CREDS_LEN, stdin))
        return -1;

    if(*username == '\n')
        return -1;
        
    char * end = strchr((char *) username, '\n');
    if(end == NULL){
        while(getc(stdin) != '\n');
        return -1;
    }
    else
        username[end-(char *) username] = 0; 

    size_t len = strlen((char *) username);
    return len;
}

static int ask_password(uint8_t * password){
    printf("Enter password: ");
    fflush(stdout);
    if(!fgets((char *) password, CREDS_LEN, stdin))
        return -1;

    if(*password == '\n')
        return -1;

    char * end = strchr((char *) password, '\n');
    if(end == NULL){
        while(getc(stdin) != '\n');
        return -1;
    }
    else
        password[end-(char *) password] = 0; 

    return strlen((char *) password);
}

static int ask_protocol(uint8_t * protocol){
    uint8_t protocol_str[NUMERIC_INPUT_LEN];
    printf("Protocol: ");
    fflush(stdout);
    if(fgets((char *) protocol_str, NUMERIC_INPUT_LEN, stdin) == 0)
        return -1;

    if(*protocol_str == '\n')
        return -1;

    char * end = strchr((char *) protocol_str, '\n');
    if(end == NULL){
        while(getc(stdin) != '\n');
        return -1;
    }
    else
        protocol_str[end-(char *)protocol_str] = 0; 
        
    errno = 0;
    char * endptr = 0;
    *protocol = strtol((char *) protocol_str, &endptr, 0);
    if(errno || *endptr)
        return -1;

    return 0;
}

static int send_credentials(int socket_fd, uint8_t * username, uint8_t * password){
    uint8_t ulen = strlen((char *) username);
    uint8_t plen = strlen((char *) password);
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
        printf("Invalid credentials, please try again");
    else{
        printf("Please enter valid credentials");
    }
}

static void print_response(uint8_t action, uint8_t method, uint16_t response_length, unsigned char * response){
    if(action == 0xFF && method == 0xFF){
        printf("\nAn error ocurred\n");
    }
    else if(action == 0xBE)
        printf("\nModification Successful!\n");
    else if(action == 0xD0){
        if(method == USER_LIST){
            int username_len;
            unsigned char username[CREDS_LEN + 1];
            while(response_length > 0){
                username_len = response[0];
                response++;
                memcpy(username, response, username_len);
                username[username_len] = 0;
                response += username_len;
                printf("%s\n", username);
                response_length -= username_len + 1;
            }
        }
        else{
            if(response_length != 8){
                printf("\nUint too big to be printed\n");
                return;
            }
            uint64_t response_value = 0;
            for(int i = 0; i < response_length; i++)
                response_value += (uint64_t) (response[i] << (8*(response_length-i-1)));
            printf("%"PRIu64"\n", response_value);
        }
    }
}

static int close_connection(int socket_fd){
    printf("Closing connection...\n");
    return close(socket_fd);
}

static void print_welcome(){
    printf("\n===========================================================\n");
    printf("Welcome to the super cool sock5 proxy configuration.\n\n");
    printf("Enter \"help\" in the command prompt to see the posible configuration commands.\n");
    printf("Enter \"quit\" in the command prompt to terminate the session.\n");
    printf("===========================================================\n");
}

//If the command is invalid it returns -1, else 0
static int resolve_command(char * command, uint8_t * action, uint8_t * method, uint8_t * parameters){

    for(int i = 0; i < QUERIES_TOTAL; i++){
        if(!strcmp(command, queries[i])){
            *action = QUERY;
            *method = i+1;
            *parameters = 0;
            return 0;
        }
    }

    for(int i = 0; i < MODIFIERS_TOTAL; i++){
        if(!strcmp(command, modifiers[i])){
            *action = MODIFY;
            *method = i;
            *parameters = 1;
            return 0;
        }
    }

    return -1;
}

static int connect_to_ipv4(struct sockaddr_in * ipv4_address){
    int sock_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(sock_fd < 0)
        return -1;

    memset(ipv4_address, 0, sizeof(*ipv4_address));
    ipv4_address->sin_family = AF_INET;
    ipv4_address->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ipv4_address->sin_port = htons(COOL_PORT);

    if(connect(sock_fd, (struct sockaddr *) ipv4_address, sizeof(*ipv4_address)) < 0){
        printf("Unable to connect\n");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

static int connect_to_ipv6(struct sockaddr_in6 * ipv6_address){
    int sock_fd = socket(AF_INET6, SOCK_STREAM , 0);
    if(sock_fd < 0)
        return -1;

    ipv6_address->sin6_family = AF_INET6;
    ipv6_address->sin6_port = htons(COOL_PORT);
    inet_pton(AF_INET6, "::1", &ipv6_address->sin6_addr);

    if(connect(sock_fd, (struct sockaddr *) ipv6_address, sizeof(*ipv6_address)) < 0){
        printf("Unable to connect\n");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

static void handle_help(int sock_fd){
    printf("Super cool socks5 proxy client\n");
    printf("Commands can be of two types, queries or modifiers\n");
    printf("Entries follow the format: <command> - <description>\n\n");
    printf("Query methods:\n");
    for(int i = 0; i < QUERIES_TOTAL; i++)
        printf("%s\n", query_description[i]);

    printf("\nModification methods:\n");
    for(int i = 0; i < MODIFIERS_TOTAL; i++)
        printf("%s\n", modifier_description[i]);
}

static void handle_quit(int sock_fd){
    close_connection(sock_fd);
    //TODO: mirar si se puede cortar la ejecucion sin usar exit, o si esta bien usar exit
    exit(0);
}