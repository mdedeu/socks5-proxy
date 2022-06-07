#ifndef PARSER_H_AUTH_COOLPROTOCOL
#define PARSER_H_AUTH_COOLPROTOCOL

struct coolProtocolAuthenticationMessage{
	uint8_t version;
	uint8_t ulen;
	char* uname;
	uint8_t passlen;
	char* pass;
};

struct coolProtocolAuthenticationResponse{
	uint16_t status;
}


struct coolProtocolAuthenticationMessage * init_coolProtocolAuthenticationMessage_parser();
bool feed_coolProtocolAuthenticationMessage_parser(struct coolProtocolAuthenticationMessage * sock_data , char * input, int input_size);
void close_coolProtocolAuthenticationMessage_parser(struct coolProtocolAuthenticationMessage * message);

#endif
