/*
 * MCP2515.c
 *
 * Created: 02.10.2019 13:59:22
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"


uint8_t MCP2515_Read(uint8_t adr)
{
	uint8_t data = 0;
	SPI_SlaveSelect();

	SPI_write(MCP_READ);
	SPI_write(adr);		// Register address
	data = SPI_read();	// Receive data from register address
		
	SPI_SlaveDeselect();
	
	return data;
}


void MCP2515_Write(char adr, uint8_t data)
{
	SPI_SlaveSelect();

	SPI_write(MCP_WRITE);
	SPI_write(adr);		// Register address
	SPI_write(data);	// Send data from register address

	SPI_SlaveDeselect();
}


// Write a single instruction
void MCP2515_cmd(char cmd)
{
	SPI_SlaveSelect();
	SPI_write(cmd);
	SPI_SlaveDeselect();
}


void MCP2515_reset( void )
{
	SPI_SlaveSelect();
	SPI_write(MCP_RESET);
	SPI_SlaveDeselect();
	
	_delay_ms(10);
}

// Status of often used status bits for RX and TX
uint8_t MCP2515_readRxTxStatus()
{
	uint8_t status;
	SPI_SlaveSelect();
	
	SPI_write(MCP_READ_STATUS);
	status = SPI_read();

	SPI_SlaveDeselect();
	return status;
}

// Read bit 7 and 6: 'message in buffer' - Read bit 4 and 3 'message-type received' - bit 2-0: 'Filter Match' 
uint8_t MCP2515_readRXstatus()
{
		uint8_t status;
		SPI_SlaveSelect();
		SPI_write(MCP_RX_STATUS);

		status = SPI_read();
		SPI_SlaveDeselect();
		return status;
}

// Request-to-Send TX0, TX1, TX2, or ALL TXbuffers
void MCP2515_RTS(uint8_t TXn)
{
	SPI_SlaveSelect();
	SPI_write(TXn);
	SPI_SlaveDeselect();
}


// Return status of register
uint8_t MCP2515_readStatus(uint8_t reg)
{
	uint8_t status = 0;
	SPI_SlaveSelect();
	SPI_write(MCP_READ);
	SPI_write(reg);
	
	status = SPI_read();
	SPI_SlaveDeselect();
	return status;
}


// Manipulate register data with bit-masking
void MCP2515_bitMask(uint8_t reg, uint8_t bitMask, uint8_t data)
{
	SPI_SlaveSelect();
	SPI_write(MCP_BITMOD);
	SPI_write(reg);
	SPI_write(bitMask); //
	SPI_write(data);
	SPI_SlaveDeselect();
}

void MCP2515init( uint8_t MODE )
{
	uint8_t state;
	MCP2515_reset();

	state = MCP2515_readStatus(MCP_CANSTAT);
	printf("CANSTAT: %x\n\r", state);
	
	if (state & MODE_MASK != MODE_CONFIG)
	{
		printf("MCP2515 NOT in configuration mode!\n\r");
	}
	 // Disable interrupt error flags and Enable TX0 and RX0 Interrupt
	MCP2515_bitMask(MCP_CANINTE, 0xff ,0x03);
	MCP2515_bitMask(MCP_RXB0CTRL, 0x64 ,0x60); // Turn off mask filters and disable Rollover
	
	// Set MODE
	MCP2515_Write(MCP_CANCTRL,MODE);	
}



void CAN_Write(char adr, CAN_message_t* data)
{
	SPI_SlaveSelect();

	SPI_write(MCP_WRITE);
	SPI_write(adr);
	SPI_write(data->ID);
	SPI_write(data->length);
	for(int i = 0; i < data->length; i++)
	{
		SPI_write(data->msg[i]);
	}

	SPI_SlaveDeselect();
}


uint8_t CAN_Read( void )
{
	uint8_t data = 0;
	SPI_SlaveSelect();

	SPI_write(MCP_READ);
	SPI_write(MCP_READ_RX0);
	
	data = SPI_read();	// Receive data
	
	SPI_SlaveDeselect();
	
	return data;
}

