#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#include <inttypes.h>
#include "ringbuff.h"

#define true 1
#define false 0
#define COMMAND_LEN 99
#define RX_BUFF_SIZE 19

typedef struct frame
{
	uint8_t frameOK;
	uint8_t src_address;
	uint8_t dst_address;
	uint8_t data_count;
	uint8_t command[COMMAND_LEN];
	uint8_t checksum;
}framecontent;


void RXbufferClear(uint8_t* buff, int size);
framecontent parseRxBuffer();

#endif /* INC_PROTOCOL_H_ */
