#ifndef PARSER_H_RESPONSE_COOLPROTOCOL
#define PARSER_H_RESPONSE_COOLPROTOCOL

struct coolProtocolResponseMessage{
	struct* coolProtocolRequestMessage;
	uint8_t rlen;
	char* response;
};

#endif