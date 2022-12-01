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
	command_t resp = {"", 0};
	strcpy(resp.TX_payload, "Unknown");

	if(strcmp(frame.command, "D1") == 0)
	{
		strcpy(resp.TX_payload, "D1");
	}
	else if(strcmp(frame.command, "D2") == 0)
	{
		strcpy(resp.TX_payload, "D2");
	}
	else if(strcmp(frame.command, "I") == 0)
	{
		strcpy(resp.TX_payload, "I");
	}
	else if(strcmp(frame.command, "CLR") == 0)
	{
		strcpy(resp.TX_payload, "CLR");
	}

	return resp;
}

