#ifndef __RINGBUFF_H__
#define __RINGBUFF_H__

#include <inttypes.h>
#define sizeOfBuffer 1024

typedef struct buffer_t {
	uint16_t TXbuffIdx;
	uint16_t RXbuffIdx;
	uint16_t ReceivedCharIdx;
	uint8_t tempArray[255];
	uint8_t tempArrayIdx;
	uint8_t array[sizeOfBuffer];
} buffer_t;

//volatile buffer_t;

#endif /*__RINGBUFF_H__*/
