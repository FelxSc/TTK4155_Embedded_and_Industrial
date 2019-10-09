/*
 * CAN.c
 *
 * Created: 05.10.2019 18:18:10
 *  Author: oddiha
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "MCP2515.h"

	uint8_t messageCount = 0x00;

void sendCANmessage(CAN_message_t* data)
{
	uint8_t IDlow, IDhigh, status, TXBnBase;

	if(messageCount > 2)
		messageCount = 0x00;	// Counter to specify the TX buffer to write the message to.
	TXBnBase = 0x30 + messageCount << 4;	// Create base address in order to change buffer. TXB0CTRL + messageCount shifted left 4 times creates base address for each of the three TX buffers
	status = MCP2515_readStatus(TXBnBase);
	printf("\n\rMessageCount: %d\n\r", messageCount);
	printf("TX status: %x\n\n\r", status);
	
	while ( (status & 0x08) ) printf("Waiting for TX buffer -%d- to be transmitted %d\n\r", messageCount);
	
	
	uint16_t ID = data->ID;
	IDhigh = ID >> 3;		// Right shift by 3 to make 8 most significant bits of the 11 bits ID
	IDlow = ID << 5;		// Left shift by 5 to make 3 Least significant bits of ID to 3 most significant bits of IDlow
	printf("ID read-for-send: %x\n\r", ID);
	
	
	_delay_ms(1);
	MCP2515_Write(TXBnBase + 0x01, IDhigh);	// TXBnBASE + offset = MCP_TXBnSIDH
	MCP2515_Write(TXBnBase + 0x02, IDlow);	// TXBnBASE + offset = MCP_TXBnSIDL
	_delay_ms(10);
	printf("IDhigh read-for-send: %x\n\r", MCP2515_Read(TXBnBase + 0x01));
	printf("IDlow read-for-send: %x\n\r", MCP2515_Read(TXBnBase + 0x02));
	
	

	
	MCP2515_Write(TXBnBase + 0x05, data->length);	// TXBnBASE + offset = MCP_TXBnDLC
	
	for(uint8_t byte = 0; byte < data->length; byte++)
	{
		MCP2515_Write(TXBnBase + 0x06+byte, data->msg[byte]);	// TXBnBASE + offset = MCP_TXBnDm
	}
	switch(messageCount)
	{
		case 0: MCP2515_RTS(MCP_RTS_TX0); break;	// Request to send TXB0
		case 1: MCP2515_RTS(MCP_RTS_TX1); break;	// Request to send TXB1
		case 2: MCP2515_RTS(MCP_RTS_TX2); break;	// Request to send TXB2
		default: break;
	}
	
	messageCount++;
	
	// Request to send
	//MCP2515_RTS(MCP_RTS_TX0);
	//MCP2515_bitMask(MCP_TXB0CTRL, 0x08, 0x08);
}

void receiveCANmesssage( CAN_message_t* data, uint8_t reg )
{
					printf("IDhigh Just before receive: %x\n\r", MCP2515_Read(MCP_RXB0SIDH));
					printf("IDlow just before receive: %x\n\r", MCP2515_Read(MCP_RXB0SIDL));
	uint16_t ID;
	
	uint8_t IDlow, IDhigh, data_length_code;

	IDhigh = MCP2515_Read(reg+0x01);	// RXBnBASE + offset = RXBnSIDH
	IDlow = MCP2515_Read(reg+0x02);		// RXBnBASE + offset = RXBnSIDL
	
	printf("\n\rIDhigh received: %x\n\r", IDhigh);
	printf("IDlow received: %x\n\r", IDlow);


	data->ID = (IDhigh << 8) | IDlow;
	data->ID = data->ID >> 5;
	printf("ID received: %x\n\r", data->ID);


/*
	IDlow = IDlow >> 5;	// shift right by 5 to get the correct value
	

	
	data->ID = IDhigh
	data->ID = data->ID << 3;
	data->ID = 0xe0 & IDlow;
	*/
	data_length_code = MCP2515_Read(reg+0x05);	// RXBnBASE + offset = MCP_RXBnDLC
	data->length = data_length_code & 0b1111;
	
	for(uint8_t byte = 0; byte < data->length; byte++)
	{
		data->msg[byte] = MCP2515_Read(reg+0x06+byte);	// RXBnBASE + offset = MCP_RXBnDm
	}
	
}