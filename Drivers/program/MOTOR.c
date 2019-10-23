/*
 * CFile1.c
 *
 * Created: 23.10.2019 15:00:18
 *  Author: oddiha
 */ 

#include "MOTOR.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <avr/delay.h>

void motorDriverInit( void )
{
	// Set motor output pins PH 1,3,4,5,6 as output
	DDRH |= 0x7A;
	
	// Set encoder input pins PK 0,1,2,3,4,5,6,7 as input
	DDRK &= ~0xFF;

	// enable motor and set initial direction
	PORTH |= (1 << PH4) | (1 << PH1);
	
	
	
}

uint16_t encoderRead( void )
{
	uint16_t data;
	 // Enable !OE and set select low
	PORTH &= ~(1 << PH5);
	PORTH |= (1 << PH3);
	
	_delay_us(40);
	
	data = (PINK << 8);
	
	PORTH &= ~(1<<PH3);
	
	_delay_us(60);
	
	data = PINK;
	
	PORTH &= ~(1 << PH6) | (1 << PH5);
	
	return data;
			
}

void setMotorSpeed(void)
{
	uint8_t messageBuffer[3];
	uint8_t adr = 0x50;
	uint8_t cmd = 0x00;
	motor.speed = 100;
	messageBuffer[0] = adr;
	messageBuffer[1] = cmd;
	messageBuffer[2] = 70;
	// I2C stuff
	 TWI_Start_Transceiver_With_Data(messageBuffer, 3 );
}

void setMotorDirection( void )
{
	switch(motor.direction)
	{
		case UP:	PORTH |= (1 << PH1); break;
		case DOWN:	PORTH &= ~(1 << PH1); break;
		default: break; // do nothing
	};
	
}

void motorDriver( void )
{
	
}