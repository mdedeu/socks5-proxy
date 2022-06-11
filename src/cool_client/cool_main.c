#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#define COOL_PORT 42069

int main(){
    uint8_t buff[100] = {1, 11, 's', 'c', 'a', 's', 't', 'a', 'g', 'n', 'i', 'n', 'o', 11, 's', 'c', 'a', 's', 't', 'a', 'g', 'n', 'i', 'n', 'o'};
    uint8_t buff_recv[100];
    
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
    printf("Connect:%d\n", ret);

    ret = send(sock_fd, buff, read_chars, 0);
    printf("Sent:%d\n", ret);

    ret = recv(sock_fd, buff_recv, 2, 0);
    printf("Recieved:%d\n", ret);

    for(int i = 0; i < 2; i++)
        printf("%d", buff_recv[i]);

    printf("\n");

}
