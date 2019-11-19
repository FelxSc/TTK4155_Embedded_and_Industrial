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
#include "CAN.h"

uint8_t messageCount = 0x00;

void sendCANmessage(CAN_message_t* data)
{
	uint8_t IDlow, IDhigh, msgLength;
	
	uint16_t ID = data->ID;
	IDlow = 0x00;			// IDlow has faulty behavior - Stuck at constant value - The register is ignored
	IDhigh = ID << 5;		// Left shift 5 places to shift bit 0, 1, and 2 up to IDbit 8, 9, and 10.
	
	
	_delay_ms(1);
	MCP2515_Write(MCP_TXB0SIDH, IDhigh);
	MCP2515_Write(MCP_TXB0SIDL, IDlow);
	MCP2515_Write(MCP_TXB0DLC, data->length);
	
	for(uint8_t byte = 0; byte < data->length; byte++)
	{
		MCP2515_Write(MCP_TXB0Dm+byte, data->msg[byte]);
	}

	// Request to send
	MCP2515_RTS(MCP_RTS_TX0);
}



void receiveCANmesssage( CAN_message_t* data)
{
	uint8_t IDlow, IDhigh, DLC;

	IDhigh = MCP2515_Read(MCP_RXB0SIDH);
	
	
	// IDlow has faulty behavior - Stuck at constant value - The register is ignored
	data->ID = IDhigh >> 5; // shift ID right 5 places to give correct ID in bit 0, 1, and 2 


	
	DLC = MCP2515_Read(MCP_RXB0DLC);
	
	data->length = DLC & 0b1111;
	
	for(uint8_t byte = 0; byte < data->length; byte++)
	{
		data->msg[byte] = MCP2515_Read(MCP_RXB0DM+byte);
	}
	
	// Set RX interrupt flag low
	MCP2515_bitMask(MCP_CANINTF, MCP_RX0IF, 0x00);
}

