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
	
	// COM1A1:	Clear output-pin on compare	- WGM: FastPWM
	TCCR1A	|=	(1 << COM1A1) | (1<<WGM11);
	TCCR1A &= ~(1<<WGM10) | (1<<COM1A0);
	
	// WGM: FastPWM	- Pre-scale: 256
	TCCR1B	|=	(1 << WGM12) | (1 << WGM13) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	
	// TimerCounter TOP value
	ICR1 = 1249;		// TOP: Number of clock signals per period
	
	// Enable timer1 Interrupt
	TIMSK1 |= (1<<TOIE1);
	
	// Initial Servo position: 94 - DutyCycle = 1.5 ms
	OCR1A = 94;
}

uint8_t calculateDutyCycleCounter( uint8_t value )
{
	// In case of inverted servo
	invertFlag = 1;
	if(invertFlag==1)
		value = 255 - value;
	
	float Temp = value;
	uint8_t dutyCycleValue = (uint8_t)(Temp/3.44) + 57;
	return dutyCycleValue;
}

void setDutyCycle( uint8_t dutyCycle )
{
	uint8_t max = 130;
	uint8_t min = 57;
	
	// Safety check: DutyCycle < 0.9, 2.1 > ms
	if(dutyCycle > max)
		dutyCycle = max;
	else if (dutyCycle < min)
		dutyCycle = min;
			
	// OutputCounter Top = dutyCycle counter
	OCR1A = (uint16_t)dutyCycle;
		 
}

