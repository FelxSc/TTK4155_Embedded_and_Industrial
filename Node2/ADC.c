/*
 * IR.c
 *
 * Created: 19.10.2019 16:34:09
 *  Author: oddiha
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>


void ADCinit( void )
{
	// Actual ADC that is used is AD0 (Pin PF0)
	DDRF &= ~(1<<PF2);

	// Enable ADC
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADMUX = (1<<REFS1) | (1<<REFS0);
}

uint16_t readADC( void )
{
	uint16_t  datalow, datahigh;
	uint16_t data;
	ADMUX &= ~(1<<MUX1)|(1<<MUX0) | (1<<MUX2) | (1<<MUX3) | (1<<MUX4);
	ADCSRA |= (1<<ADSC);
	
	loop_until_bit_is_clear(ADCSRA,ADSC); 
	
	datalow = ADCL;
	datahigh = (ADCH<<8);
	data = datahigh + datalow;
	
	return data;
}