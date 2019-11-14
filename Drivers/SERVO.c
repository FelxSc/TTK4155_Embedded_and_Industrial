/*
 * SERVO.c
 *
 * Created: 16.10.2019 15:37:13
 *  Author: oddiha
 */ 

#define F_CPU 16000000UL // 16 Mhz


#include <avr/io.h>

#include "SERVO.h"

void ServoTimer1Init( void )
{
	
	DDRB |= (1<<PB5);
	
	
	
	// COM1A1:	Clear output-pin on compare				-- WGM: FastPWM
	TCCR1A	|=	(1 << COM1A1) | (1<<WGM11);
	TCCR1A &= ~(1<<WGM10) | (1<<COM1A0);
	
	// WGM: FastPWM			-- Prescale: 256
	TCCR1B	|=	(1 << WGM12) | (1 << WGM13) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	
	// TimerCounter TOP value
	ICR1 = 1249;		// TOP: Number of clocksignals per period
	
	TIMSK1 |= (1<<TOIE1);
	

	OCR1A = 94;	// Initial Servo position: 94 clk cycles = 1.5 ms
	
}

void setDutyCycle( uint8_t count )
{
	uint8_t maxCount = 131;
	uint8_t minCount = 57;
	
	// Safety check for within the dutyCycle <0.9 ms, 2.1ms>
	if(count > 130)
		count = 130;
	else if (count < 57)
		count = 57;
		
	OCR1A = (uint16_t)count;	// OutputCounter reg to dutyCycle counter 
	printf("x: %d",count);	
}

uint8_t calculateDutyCycleCounter( uint8_t number )
{
	invertFlag = 1;
	if(invertFlag==1)
		number = 255 - number;
	//printf("x: %d",number);
	float floatingnumber = number;
	uint8_t cycleCounts = (uint8_t)(floatingnumber/3.44) + 57;
	return cycleCounts;
}

