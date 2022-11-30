/*
 * commands.c
 *
 *  Created on: Feb 26, 2022
 *      Author: Adrian
 */

#include "commands.h"
#include <string.h>
#include <stdlib.h>

extern volatile buffer_t Rx, Tx;
extern framecontent frame;

command_t commands()
{
	command_t comm = {"", 0};

	if(strcmp(frame.command, "D1") == 0)
	{
		USART_Send("D1");
	}
	else if(strcmp(frame.command, "D2") == 0)
	{
		USART_Send("D2");
	}
	else if(strcmp(frame.command, "I") == 0)
	{
		USART_Send("I");
	}
	else if(strcmp(frame.command, "CLR") == 0)
	{
		USART_Send("CLR");
	}
	else
	{
		USART_Send("Unknown");
	}

	return comm;
}

