/*
 * adcTest.c
 *
 * Created: 11.09.2019 16:13:05
 *  Author: oddiha
 */ 


#define F_CPU 4915200 // 4.9152 Mhz
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>

#include "adc.h"


typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} channel_t;		// datatype to select adc channels

volatile uint8_t * const extADC = (uint8_t)ADC_ADDRESS;
volatile uint8_t ADCdata;

void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}


void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
	//DDRC |= 0xff;	// set Port C 0 - 3 to Output
	//DDRE |= (1<<DDE1);
	//PORTE = 0x2;
	
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<< UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
	
}

void adcInit( void )
{
	// PortE pin0 as input
	DDRE &= ~(1 << PE0);
	
	//disable global interrupt
	//cli();
	
	//Interrupt on falling edge
	//EMCUCR &= ~(1<<ISC2);
	
	//Enable interrupt on PE0
	//GICR |= (1<<INT2);
	
	//enable global interrupt
	//sei();
	
	
	
}

ISR(INT2_vect)
{
	ADCdata = extADC;
}

void select_ADC_channel(channel_t channel)
{
	switch(channel){
		case CHANNEL1:
			extADC[0x00] = 0x04;
			break;
		case CHANNEL2:
			extADC[0x00] = 0x05;
			break;
		case CHANNEL3:
			extADC[0x00] = 0x06;
			break;
		case CHANNEL4:
			extADC[0x00] = 0x07;
			break;
		default:
			printf("Invalid channel");
	}
	
}

uint8_t getADCdata(void)
{
	while(PINE & (1<<PE0));
	_delay_us(60);
	ADCdata = extADC[0x04];
	return ADCdata;
	
}

uint8_t ADC_read(uint8_t channel) {
	volatile uint8_t *adc = (uint8_t *) ADC_ADDRESS;
	_delay_us(1);
	adc[channel] = channel;
	while (PINE & (1<<PE0));  //Wait until ADC conversion process finishes (supposedly 40us according to datasheet)

	_delay_us(60);     //(at least 600ns according to datasheet)

	return *adc;
}

int main(void) 
{
	
		USART_Init ( MYUBRR );
		fdevopen(&USART_Transmit, &USART_Receive);
		ExernalMemoryInit();
		adcInit();
		
		printf("Start Test");
		
		channel_t channel = CHANNEL1;
    while(1)
    {
		if(PIND&0x08)
		{
			printf("Button WORKS!\n\r");
		}
		ADCdata = ADC_read(CHANNEL2);
		//ADCdata = ADC_data();
		printf("%d\n\r", ADCdata);
       _delay_ms(1);
    }
}