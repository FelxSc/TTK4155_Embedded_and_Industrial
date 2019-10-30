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

	//PID_t pid;


void motorDriverInit( void )
{
	// Set motor output pins PH 1,3,4,5,6 as output
	DDRH |= 0x7A;
	
	// Set encoder input pins PK 0,1,2,3,45,,6,7 as input
	DDRK &= ~0xFF;

	// enable motor				 and set initial direction
	PORTH |= (1 << PH4);		// | (1 << PH1);
	
	setMotorDirection(true, DOWN);
	setMotorSpeed(true, 0);
	
}

uint16_t encoderRead( void )
{
	uint16_t data,dataHigh, dataLow;
	 // Enable !OE and set select low
	PORTH &= ~(1 << PH5);	// OE!	LOW: Output enabled
	PORTH &= ~(1 << PH3);	// SEL	LOW: Get high byte
	
	_delay_us(40);
	
	dataHigh = PINK;		// read high byte
	//printf("Read High: %x\n\r",dataHigh);
	
	PORTH |= (1<<PH3);		// SEL HIGH: Get low byte
	
	_delay_us(60);
	
	dataLow = PINK;			//	read low byte 
	//printf("Read Low: %x\n\r",dataLow);
	
	//encoderReset();
	
	PORTH |= (1 << PH5);	// OE! HIGH: disable output of encoder
	
	data = (dataHigh << 8)| (dataLow & 0xff);

	return data;
			
}

void encoderReset( void )
{
	// Toggle !RST: Reset encoder
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

void setMotorDirection( bool calibrateFlag, uint8_t motorDirection )
{
	if(calibrateFlag)
		motor.direction = motorDirection;
	
	switch(motor.direction)
	{
		case UP:	PORTH |= (1 << PH1); break;
		case DOWN:	PORTH &= ~(1 << PH1); break;
		default: break; // do nothing
	};
	
}


uint16_t motorCalibrate( void )
{
	uint16_t counter = 0;
	
	setMotorDirection(true, UP);
	setMotorSpeed(true, 70);
	_delay_ms(1000);
	
	encoderReset();
	
	setMotorDirection(true, DOWN);
	setMotorSpeed(true, 70);	
	
	_delay_ms(1200);
	counter = encoderRead();
	pid.maxDistance = counter;
	printf("Counter = %d\n\r",counter);
	
	setMotorDirection(true, UP);
	setMotorSpeed(true, 70);
	_delay_ms(1200);
	pid.minDistance = counter;
		printf("Counter = %d\n\r",counter);
	
	counter = counter/2; // Calculate the average and the middle of the board in terms of motor turns
	printf("Counter = %d\n\r",counter);
	
	
	return counter;
}


void motorDriver( void )
{
	// PID controller
	
		
	
}