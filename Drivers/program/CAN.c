/*
 * CAN.c
 *
 * Created: 05.10.2019 18:18:10
 *  Author: oddiha
 */ 

#define F_CPU 16000000UL // 16 Mhz

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "MCP2515.h"

	uint8_t messageCount = 0x00;

void sendCANmessage(CAN_message_t* data)
{
	uint8_t IDlow, IDhigh, status, nrOfBytes;

	status = MCP2515_readStatus(MCP_TXB0CTRL);
	//printf("TX status: %x\n\n\r", status);
	
	while ( (status & 0x08) ) printf("Waiting for TX0 buffer\n\r"); // CHANGE THIS WHILE LOOP, MCU gets stuck in here
	
	
	uint16_t ID = data->ID;
	IDlow = 0x00;		// RXB0IDL does not work .. Therfore we can not send IDlow - Send 0x00.
	IDhigh = ID << 5;		// IDlow does not work .. Use RX0BIDH to read Send and Receive ID. Left shift 5 places to shift bit 0, 1, and 2 up to IDbit 8, 9, and 10.
	
	
	_delay_ms(1);
	MCP2515_Write(MCP_TXB0SIDH, IDhigh);	// TXBnBASE + offset = MCP_TXBnSIDH
	MCP2515_Write(MCP_TXB0SIDL, IDlow);	// TXBnBASE + offset = MCP_TXBnSIDL

	

	
	MCP2515_Write(MCP_TXB0DLC, data->length);	// TXBnBASE + offset = MCP_TXBnDLC
	nrOfBytes = data->length;
	//printf("nrOfBytes : %d",nrOfBytes);
	
	for(uint8_t byte = 0; byte < nrOfBytes; byte++)
	{
		MCP2515_Write(MCP_TXB0Dm+byte, data->msg[byte]);	// TXBnBASE + offset = MCP_TXBnDm
	}

	// Request to send
	MCP2515_RTS(MCP_RTS_TX0);
}



void receiveCANmesssage( CAN_message_t* data, uint8_t reg )
{
	uint16_t ID;
	uint8_t IDlow, IDhigh, data_length_code, nrOfBytes;

	IDhigh = MCP2515_Read(MCP_RXB0SIDH);	// RXBnBASE + offset = RXBnSIDH
	//IDlow = MCP2515_Read(MCP_RXB0SIDL);		// RXBnBASE + offset = RXBnSIDL ---- IDlow does not work
	

	//data->ID = /*(IDhigh << 8) |*/ IDlow; ---- ID low does not work
	data->ID = IDhigh >> 5; // shift received ID right 5 places to give correct ID in bit 0, 1, and 2 
	//printf("ID received: %x\n\r", data->ID);


	
	data_length_code = MCP2515_Read(MCP_RXB0DLC);	// RXBnBASE + offset = MCP_RXBnDLC
	nrOfBytes = data_length_code & 0b1111;
	
	data->length = data_length_code & 0b1111;
	//printf("nrOfBytes : %d",nrOfBytes);
	
	for(uint8_t byte = 0; byte < nrOfBytes; byte++)
	{
		data->msg[byte] = MCP2515_Read(MCP_RXB0DM+byte);	// RXBnBASE + offset = MCP_RXBnDm
	}
	MCP2515_bitMask(MCP_CANINTF, MCP_RX0IF, 0x00);
}

