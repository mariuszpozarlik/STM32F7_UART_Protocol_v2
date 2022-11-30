#ifndef __RINGBUFF_H__
#define __RINGBUFF_H__

#include <inttypes.h>
#define sizeOfBuffor 1024
#define RING_BUF_SIZE 70
#define OK 1
#define NOK 0
#define BUFF_EMPTY 0
#define BUFF_NOT_EMPTY 1
#define BUFF_NOT_OVF 0
#define BUFF_OVF 1
//#define NULL (void*)0

typedef struct flags_t
{
	uint8_t clr_flag : 1;
	uint8_t buffer_overflow_flag : 1;
}flags_t;

typedef struct buffer_t {
	uint16_t busy;
	uint16_t empty;
	uint16_t beginIdx;
	uint16_t endIdx;
	uint8_t tempArray[255];
	uint8_t tempIdx;
	uint8_t array[sizeOfBuffor];
} buffer_t;

//volatile buffer_t;

#endif /*__RINGBUFF_H__*/
