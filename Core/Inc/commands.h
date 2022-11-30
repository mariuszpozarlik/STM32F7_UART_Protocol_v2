/*
 * commands.h
 *
 *  Created on: Feb 26, 2022
 *      Author: Adrian
 */

#include "ringbuff.h"
#include "protocol.h"

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

#define COMM_LEN 10

typedef struct command
{
	uint8_t TX_payload[100];
	uint16_t len;
}command_t;

command_t commands();
void USART_Send(char* message, ...);

#endif /* INC_COMMANDS_H_ */
