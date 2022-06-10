#ifndef PARSER_H_REQUEST_LISTENER_COOLPROTOCOL
#define PARSER_H_REQUEST_LISTENER_COOLPROTOCOL

struct coolProtocolRequestMessage{
	
	uint8_t action;
	uint8_t method;

};

struct coolProtocolRequestMessageAddUser{
	
	struct coolProtocolRequestMessage * request;
	uint8_t ulen;
	char* uname;
	uint8_t passlen;
	char* pass;

};

struct coolProtocolRequestMessageRemoveUser{
	
	struct coolProtocolRequestMessage * request;
	uint8_t ulen;
	char* uname;

};


struct coolProtocolRequestMessagePassSpoofing{
	
	struct coolProtocolRequestMessage * request;
	uint8_t pcol;

};

struct coolProtocolRequestMessageChangeBufferSize{
	
	struct coolProtocolRequestMessage * request;
	uint16_t nsize;
		
};

//parser intializations

struct coolProtocolRequestMessage * init_coolProtocolRequest_parser();
struct coolProtocolRequestMessageAddUser * init_coolProtocolRequestAddUser_parser();
struct coolProtocolRequestMessageRemoveUser * init_coolProtocolRequestRemoveUser_parser();
struct coolProtocolRequestMessagePassSpoofing * init_coolProtocolRequestPassSpoofing_parser();
struct coolProtocolRequestMessageChangeBufferSize * init_coolProtocolRequestChangeBufferSize_parser();

//parser feeds

bool feed_coolProtocolRequestMessage_parser(struct coolProtocolRequestMessage * sock_data , char * input, int input_size);
bool feed_coolProtocolRequestMessageAddUser_parser(struct coolProtocolRequestMessageAddUser* sock_data , char * input, int input_size);
bool feed_coolProtocolRequestMessageRemoveUser_parser(struct coolProtocolRequestMessageRemoveUser * sock_data , char * input, int input_size);
bool feed_coolProtocolRequestMessagePassSpoofing_parser(struct coolProtocolRequestMessagePassSpoofing * sock_data , char * input, int input_size);
bool feed_coolProtocolRequestMessageChangeBufferSize_parser(struct coolProtocolRequestMessageChangeBufferSize * sock_data , char * input, int input_size);


//parser closing methods

void close_coolProtocolRequestMessage_parser(struct coolProtocolRequestMessage * message);
void close_coolProtocolRequestMessageAddUser_parser(struct coolProtocolRequestMessageAddUser * message);
void close_coolProtocolRequestMessageRemoveUser_parser(struct coolProtocolRequestMessageRemoveUser * message);
void close_coolProtocolRequestMessagePassSpoofing_parser(struct coolProtocolRequestMessagePassSpoofing * message);
void close_coolProtocolRequestMessageChangeBufferSize_parser(struct coolProtocolRequestMessageChangeBufferSize * message);


#endif
