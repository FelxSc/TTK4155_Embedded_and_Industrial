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

void InterruptInit( void )
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



void Timer1Init( void )
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

void Timer3Init( void )
{
	// Set normal mode
	TCCR3A = 0x00;
	
	// Control register: Prescale = 1024
	TCCR3B = 0x0D;
	
	// Output Compare register Value = 4800 tick/sec
	OCR3AH = 0x12;
	OCR3AL = 0xc0;
	
	// disable TIMER3 Output Compare Interrupt
	//ETIMSK &= ~(1 << OCIE3A);
	ETIMSK |= (1<<OCIE3A);
	//enableGameTimer();
	
	// Initiate TOV3 flag as cleared
	ETIFR = 0x00;
	
}

void enableGameTimer( void )
{
	cli();
	// Enable TIMER3 Output Compare Interrupt
	ETIMSK |= (1 << OCIE3A);
	
	// Initiate TOV3 flag as cleared
	ETIFR = 0x00;
	sei();	
}

void disableGameTimer( void )
{
	cli();
	// disable TIMER3 Output Compare Interrupt
	ETIMSK &= ~(1 << OCIE3A);
	sei();
}


void SendTimer1CANmsg( void )
 {
	 Timer1_interrupt = 0;
 		CAN_message_t data;
		 sliderDriver();
		 
		 data.ID = 1;
		 data.msg[0] = slider_data.leftslider; // Servo data -> dutycycle
		 data.msg[1] = slider_data.rightslider; // motor position data
		 data.msg[2] = slider_data.leftbutton;
		 data.msg[3] = gameStatus;
		 data.msg[4] = '0';
		 data.msg[5] = '0';
		 data.msg[6] = '0';
		 data.msg[7] = '\0';
		 data.length = 8;
		 
		  
	 	//printf("\n\n\r************SENDING MSG: data *************\n\r");

	 	// CAN struct test
	 	/*printf("ID: %d\n\r",data.ID);
	 	printf("X position: %d\n\r", data.msg[0]);
	 	printf("Y position: %d\n\r", data.msg[1]);
	 	printf("Button: %d\n\r", data.msg[2]);
	 	printf("msgLen: %d\n\r",data.length);*/
	 	sendCANmessage(&data);
		 
		//printf("TX status: %x",MCP2515_readRxTxStatus());
 }
 
 CAN_message_t handleCANreceiveInterrupt()
 {
	 CAN_interrupt = 0;
	 printf("\n\nr\rCAN Interrupt");
	 
	 CAN_message_t receivedCAN1;
	 
	 //CAN_message_t * receivedCAN1 = malloc(sizeof(CAN_message_t));
	 //CAN_message_t * receivedCAN2 = malloc(sizeof(CAN_message_t));
	 
	 //receivedCAN1 = received1;
	 //receivedCAN2 = received2;
	 uint8_t status;
	 
	 // Interrupt status
	 status = MCP2515_Read(MCP_CANINTF);
	 printf("\n\rCANINTF : %x\n\n\r", status);
	 cli();
	 receiveCANmesssage(&receivedCAN1, 0x60);
	 sei();
	 return receivedCAN1;
 }