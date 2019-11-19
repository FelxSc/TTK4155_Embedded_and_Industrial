/*
 * CAN.c
 *
 * Created: 05.10.2019 18:18:10
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "MCP2515.h"
#include "CAN.h"

void sendCANmessage(CAN_message_t* data)
{
	uint8_t IDlow, IDhigh, status, nrOfBytes;

	status = MCP2515_readStatus(MCP_TXB0CTRL);
	//printf("TX status: %x\n\n\r", status);
	
	while ( (status & 0x08) ) printf("Waiting for TX0 buffer\n\r");
	
	
	uint16_t ID = data->ID;
	IDlow = 0x00;			// IDlow has faulty behavior - Stuck at constant value - The register is ignored
	IDhigh = ID << 5;		// Left shift 5 places to shift bit 0, 1, and 2 up to IDbit 8, 9, and 10.
	
	
	_delay_ms(1);
	MCP2515_Write(MCP_TXB0SIDH, IDhigh);
	MCP2515_Write(MCP_TXB0SIDL, IDlow);

	
	MCP2515_Write(MCP_TXB0DLC, data->length);
	nrOfBytes = data->length;

	
	for(uint8_t byte = 0; byte < nrOfBytes; byte++)
	{
		MCP2515_Write(MCP_TXB0Dm+byte, data->msg[byte]);
	}

	// Request to send
	MCP2515_RTS(MCP_RTS_TX0);
	MCP2515_bitMask(MCP_CANINTF, MCP_TX0IF, 0x00);
}



void receiveCANmesssage( CAN_message_t* data)
{
	uint8_t IDhigh, data_length_code, nrOfBytes;

	IDhigh = MCP2515_Read(MCP_RXB0SIDH);
	

	// IDlow has faulty behavior - Stuck at constant value - The register is ignored
	data->ID = IDhigh >> 5; // shift ID right 5 places to give correct ID in bit 0, 1, and 2

	
	data_length_code = MCP2515_Read(MCP_RXB0DLC);
	nrOfBytes = data_length_code & 0b1111;
	
	data->length = data_length_code & 0b1111;
	
	for(uint8_t byte = 0; byte < nrOfBytes; byte++)
	{
		data->msg[byte] = MCP2515_Read(MCP_RXB0DM+byte);
	}
	MCP2515_bitMask(MCP_CANINTF, MCP_RX0IF, 0x00);
	MCP2515_bitMask(MCP_CANINTF, MCP_RX1IF, 0x00);
}

