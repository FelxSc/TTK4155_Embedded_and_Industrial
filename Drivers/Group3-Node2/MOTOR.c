/*
 * CFile1.c
 *
 * Created: 23.10.2019 15:00:18
 *  Author: oddiha
 */ 
#define F_CPU 16000000UL // 16 Mhz


#include "MOTOR.h"
#include "TWI_Master.h"
#include "pid.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>


void motorDriverInit( void )
{
	// Set motor output pins PH 1,3,4,5,6 as output
	DDRH |= 0x7A;
	
	// Set encoder input pins PK 0,1,2,3,4,5,6,7 as input
	DDRK &= ~0xFF;

	// enable motor
	PORTH |= (1 << PH4);
	
	setMotorDirection(true, LEFT);
	setMotorSpeed(true, 0);
}

uint16_t encoderRead( void )
{
	uint16_t data,dataHigh, dataLow;
	 
	// Enable !OE and set select to low
	PORTH &= ~(1 << PH5);	// OE!	LOW: Output enabled
	PORTH &= ~(1 << PH3);	// SEL	LOW: Get dataHigh byte
	
	_delay_us(40);
	
	// read dataHigh byte
	dataHigh = PINK;
	
	PORTH |= (1<<PH3);		// set select to HIGH to read read dataLow byte
	
	_delay_us(60);
	
	// read dataLow byte
	dataLow = PINK; 
	
	// disable encoder output
	PORTH |= (1 << PH5);
	
	data = (dataHigh << 8)| (dataLow & 0xff);

	return data;
}

void encoderReset( void )
{
	// Toggle !RST to Reset encoder
	PORTH &= ~(1 << PH6);
	_delay_us(200);
	PORTH |= (1 << PH6);
}

void setMotorSpeed( bool calibrateFlag, uint8_t motorSpeed )
{
	if(calibrateFlag)
		motor.speed = motorSpeed;
	
	uint8_t messageBuffer[3];
	uint8_t adr = 0x50;
	uint8_t cmd = 0x00;
	
	messageBuffer[0] = adr;
	messageBuffer[1] = cmd;
	messageBuffer[2] = motor.speed;
	
	// Send I2C message
	 TWI_Start_Transceiver_With_Data(messageBuffer, 3 );
}

void setMotorDirection( bool forceDirection, uint8_t motorDirection )
{
	// In case direction is set other than from CAN message
	if(forceDirection)
		motor.direction = motorDirection;
	
	switch(motor.direction)
	{
		case RIGHT:	PORTH |= (1 << PH1); break;
		case LEFT:	PORTH &= ~(1 << PH1); break;
		default: break; // do nothing
	};
	
}


uint16_t motorCalibrate( void )
{
	uint16_t counter = 0;
	
	setMotorDirection(true, RIGHT);
	setMotorSpeed(true, 70);
	_delay_ms(1000);
	
	encoderReset();
	
	setMotorDirection(true, LEFT);
	setMotorSpeed(true, 70);	
	
	_delay_ms(1200);
	counter = encoderRead();
	pid.maxDistance = counter;
	printf("Counter: Left = %d\n\r",counter);
	
	setMotorDirection(true, RIGHT);
	setMotorSpeed(true, 70);
	_delay_ms(1200);
	pid.minDistance = counter;
		printf("Counter: Right = %d\n\r",counter);
		
	// Calculate average = middle of the board in terms of motor turns
	counter = counter/2;
	printf("Counter = %d\n\r",counter);
	
	return counter;
}