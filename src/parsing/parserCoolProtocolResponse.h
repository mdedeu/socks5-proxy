#ifndef PARSER_H_RESPONSE_COOLPROTOCOL
#define PARSER_H_RESPONSE_COOLPROTOCOL

#include <stdint.h>

struct coolProtocolResponseMessage{
	struct coolProtocolRequestMessage * request;
	uint8_t rlen;
	char* response;
};

#endif