/*
 * adcTest.c
 *
 * Created: 11.09.2019 16:13:05
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "ADC.h"
#include "JOYSTICK.h"
	
volatile uint8_t* extADC = (uint8_t*) ADC_ADDRESS;
volatile uint8_t ADCdata;



void adcInit( void )
{
	// PortE pin0 as input
	DDRE &= ~(1 << PE0);
	
	// PortD pin 2 and 3 as input
	DDRD &= ~(1 << PD2);	// MCP2515 Interrupt
	DDRD &= ~(1 << PD3);	// JOYSTICK PUSHBUTTON
	
	// Set pull-up resistor PE0 & PB0
	PORTE |= (1 << PE0);	// LatchEnable Active Low 
	
	// pull-down
	PORTD &= ~(1 << PD2);	// MCP2515 interrupt
	PORTD &= ~(1 << PD3);	//	Joystick PUSHBUTTON	
}



void select_ADC_channel(channel_t channel)
{	
	switch(channel){
		case CHANNEL1:
			*extADC = (uint8_t ) 0x04;
			break;
		case CHANNEL2:
			*extADC = (uint8_t ) 0x05;
			break;
		case CHANNEL3:
			*extADC = (uint8_t ) 0x06;
			break;
		case CHANNEL4:
			*extADC = (uint8_t ) 0x07;
			break;
		default:
			printf("Invalid channel");
	}
	
}

uint8_t getADCdata(channel_t channel)
{
	select_ADC_channel(channel);
	while (PINE & (1<<PE0));	// Wait for ADC data
	ADCdata = *extADC;
	return ADCdata;
}