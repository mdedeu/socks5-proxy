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

void print_status(uint16_t status);
void print_response(uint8_t action, uint8_t method, uint8_t response_length, char * response);

int main(){
    uint8_t buff[100] = {1, 11, 's', 'c', 'a', 's', 't', 'a', 'g', 'n', 'i', 'n', 'i', 11, 's', 'c', 'a', 's', 't', 'a', 'g', 'n', 'i', 'n', 'o'};
    uint8_t buff_recv[RECV_BUFFER_SIZE];
    
    int read_chars = 25;
    int ret;
    //int read_chars = read(0, buff, 100);
    int sock_fd = socket(AF_INET , SOCK_STREAM , 0);
    printf("Fd:%d\n", sock_fd);

    if(sock_fd < 0)
        printf("ERROR\n");

    struct sockaddr_in server_address_4;
    memset(&server_address_4, 0, sizeof(server_address_4));
    server_address_4.sin_family      = AF_INET;
    server_address_4.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address_4.sin_port        = htons(COOL_PORT);

    ret = connect(sock_fd, (struct sockaddr *) &server_address_4, sizeof(server_address_4));

    ret = send(sock_fd, buff, read_chars, 0);

    while(1){

    struct simple_response_message * simple_response = init_simple_response_parser();

    int read_amount;
    do{
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_general_response_parser(simple_response, buff_recv, read_amount));

    uint16_t returned_status = simple_response->status[0] << 8;
    returned_status += simple_response->status[1];
    
    print_status(returned_status);

    close_simple_response_parser(simple_response);

    struct general_response_message * general_response = init_general_response_parser();

    do{
        read_amount = recv(sock_fd, buff_recv, RECV_BUFFER_SIZE, 0);
        if(read_amount < 0)
            return 1;
    } while(!feed_general_response_parser(general_response, buff_recv, read_amount));

    print_response(general_response->action, general_response->method, general_response->response_length, general_response->response);

    close_general_response_parser(general_response);

    }
}


void print_status(uint16_t status){
    printf("Status: %d\n", status);
}

void print_response(uint8_t action, uint8_t method, uint8_t response_length, char * response){
    printf("Action: %d\n", action);
    printf("Method: %d\n", method);
    *(response + response_length) = 0;
    printf("Response: %s", response);
}