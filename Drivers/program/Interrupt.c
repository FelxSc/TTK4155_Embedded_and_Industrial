/*
 * CFile1.c
 *
 * Created: 13.11.2019 15:19:03
 *  Author: felixas
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>

#include "Interrupt.h"
#include "JOYSTICK.h"
#include "CAN.h"
#include "MCP2515.h"
#include "game.h"



void receiveCanInterruptInit( void )
{
	DDRD &= ~(1<<PD2); // MCP2515 interrupt
	
	cli();
	
	// Interrupt on falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);

	
	// Enable Interrupt on PD2
	GICR |= (1<<INT0);
	
	// Enable global interrupts
	sei();
}


void buzzerTimer0Init( void )
{
	DDRB |= ( 1<< PB0);

	cli();
	// Set normal mode fastPWM - Toggle OC0 on Compare Match - No Prescaling
	TCCR0 |= (1 << WGM01) | (1 << COM00) | (1 << CS01);// | (1 << CS00);
	//TCCR0 &= ~((1 << WGM00) | (1 << CS01) | (1 << FOC0));
	
	// Initial compare value
	OCR0 = 10;

	// Enable TIMER0 Interrupt
	TIMSK |= (1 << OCIE0);
	
	// Initiate OCF0 flag as cleared
	TIFR &= ~(1 << OCF0);
	sei();
}


//Send CAN message
void SendCANTimer1Init( void )
{
	// Set normal mode
	TCCR1A = 0x00;
	
	// Control register: Prescale = 8
	TCCR1B = 0x02;
	
	// Enable TIMER1 Interrupt
	TIMSK = 0x80;
	
	// Initiate TOV1 flag as cleared
	TIFR = 0x00;
	
}

void SendCAN( void )
{
	cli();
	Timer1_interrupt = 0;
	CAN_message_t data;
	sliderDriver();
	
	data.ID = 1;
	data.msg[0] = slider_data.leftslider; // Servo data -> dutycycle
	data.msg[1] = slider_data.rightslider; // motor position data
	data.msg[2] = slider_data.leftbutton;
	data.msg[3] = gameStatus;
	data.msg[4] = 0;
	data.msg[5] = 0;
	data.msg[6] = 0;
	data.msg[7] = '\0';
	data.length = 8;
	
	sendCANmessage(&data);
	
	sei();
}

void enableSendCANTimer1( void )
{
	// Control register: Prescale: 8
	TCCR1B = 0x02;
}

void disableSendCANTimer1( void )
{
	// Control register: Timer/Counter stopped
	TCCR1B = 0x00;
}



// Game timer increases scorePoints every second
void gameTimer3Init( void )
{
	cli();
	// Set normal mode
	TCCR3A = 0x00;
	
	// Control register: Prescale = 1024
	TCCR3B = 0x0D;
			// Control register: Prescale: 1
			//TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);
			//TCCR3B &= ~(1 << CS31);
	
	// Output Compare register Value = 4800 tick/sec
	OCR3AH = 0x12;
	OCR3AL = 0xc0;
	//		OCR3AH = 0x13;
	//		OCR3AL = 0x33;
	
	//		OCR3AH = 0x00;
	//		OCR3AL = 0x05;

	ETIMSK |= (1<<OCIE3A);
	
	// Initiate TOV3 flag as cleared
	ETIFR = 0x00;
	sei();
}

void Timer3_millisecond(void)
{		cli();
		// Control register: Prescale: 1
		//TCCR3B |= (1 << WGM32) | (1 << CS30);
		//TCCR3B &= ~(1 << CS31) | (1 << CS32);
		
		// Output Compare register Value = 4800 tick/sec
		OCR3AH = 0x00;
		OCR3AL = 0x05;
		sei();
}
void Timer3_second(void)
{
		// Control register: Prescale: 1024
		TCCR3B = 0x0D;
		// Output Compare register Value = 4800 tick/sec
		OCR3AH = 0x12;
		OCR3AL = 0xc0;
}

// Start counting score
void enableGameTimer3( void )
{
	cli();
	
	TCCR3B = 0x0D;					// Control register: Prescale = 1024
	ETIMSK |= (1 << OCIE3A);		// Enable TIMER3 Output Compare Interrupt
	
	ETIFR = 0x00;					// Initiate TOV3 flag as cleared
	sei();	
}


// Stop counting score
void disableGameTimer3( void )
{
	cli();
	
		TCCR3B = 0x08;					// Control register: Timer/Counter stopped
		ETIMSK &= ~(1 << OCIE3A);		// disable TIMER3 Output Compare Interrupt
	sei();
}



 CAN_message_t handleCANreceiveInterrupt()
 {
	 CAN_interrupt = 0;
	 CAN_message_t receivedCAN1;
	 
	 receiveCANmesssage(&receivedCAN1, 0x60);
	 
	 return receivedCAN1;
 }