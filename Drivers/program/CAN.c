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



void sendCANmessage(CAN_message_t* data)
{
	uint8_t IDlow, IDhigh;
	

	//MCP2515_bitMask(MCP_TXB0CTRL, 0x08, 0x00); // Check if this is correct.......!!
	
	uint16_t ID = data->ID;
	IDhigh = ID >> 3;		// Right shift by 3 to make 8 most significant bits of the 11 bits ID
	IDlow = ID << 5;		// Left shift by 5 to make 3 Least significant bits of ID to 3 most significant bits of IDlow
	printf("ID read-for-send: %x\n\r", ID);
	
	
	_delay_ms(1);
	MCP2515_Write(MCP_TXB0SIDH, IDhigh);
	MCP2515_Write(MCP_TXB0SIDL, IDlow);
	_delay_ms(10);
	printf("IDhigh read-for-send: %x\n\r", MCP2515_Read(MCP_TXB0SIDH));
	printf("IDlow read-for-send: %x\n\r", MCP2515_Read(MCP_TXB0SIDL));
	
	

	
	MCP2515_Write(MCP_TXB0DLC, data->length);
	
	for(uint8_t i = 0; i < data->length; i++)
	{
		MCP2515_Write(MCP_TXB0Dm+i, data->msg[i]);
	}
	
	// Request to send
	MCP2515_RTS(MCP_RTS_TX0);
	//MCP2515_bitMask(MCP_TXB0CTRL, 0x08, 0x08);
}

void receiveCANmesssage( CAN_message_t* data )
{
					printf("IDhigh Just before receive: %x\n\r", MCP2515_Read(MCP_RXB0SIDH));
					printf("IDlow just before receive: %x\n\r", MCP2515_Read(MCP_RXB0SIDL));
	uint16_t ID;
	
	uint8_t IDlow, IDhigh, data_length_code;

	IDhigh = MCP2515_Read(MCP_RXB0SIDH);	
	IDlow = MCP2515_Read(MCP_RXB0SIDL);
	
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
	data_length_code = MCP2515_Read(MCP_RXB0DLC);
	data->length = data_length_code & 0b1111;
	
	for(uint8_t i = 0; i < data->length; i++)
	{
		data->msg[i] = MCP2515_Read(MCP_RXB0DM+i);
	}
	
}