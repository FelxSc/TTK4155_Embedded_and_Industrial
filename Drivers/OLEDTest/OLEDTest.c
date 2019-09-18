/*
 * OLEDTest.c
 *
 * Created: 18.09.2019 17:08:55
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
#include <stdint.h>
#include <avr/pgmspace.h>

#include "OLED.h"


volatile uint8_t* extOledCmd = (uint8_t*) OLED_COMMAND_ADDRESS;
volatile uint8_t* extOledData = (uint8_t*) OLED_DATA_ADDRESS;

void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
	
}

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

void write_c(uint8_t command)
{
	*extOledCmd = command;
}

void write_d(uint8_t data)
{
	*extOledData = data;
}

void OledInit()
{
	write_c(0xae);        // turn display  off
	write_c(0xa1);        // segment  remap: column address 127 is mapped to SEG0
	write_c(0xda);        // common  pads  hardware:  alternative
	write_c(0x12);        // set the higher nibble pf the column start address register for Page Addressing Mode using X[0:3] as data bits
	write_c(0xc8);        // common  output scan direction: com63~com0
	write_c(0xa8);        // multiplex ratio mode: 63, it is set next line
	write_c(0x3f);        // Value of multiplex ratio set to 0x3f (63)
	write_c(0xd5);        // display  divide ratio/osc. freq. mode, it is set next line
	write_c(0x80);        // Value of display divide ratio/osci: A[7:4] Osci. Frequency, A[3:0] display clock divide ratio
	write_c(0x81);        // contrast  control
	write_c(0x50);        // Value of contrast
	write_c(0xd9);        // set  pre-charge  period
	write_c(0x21);        // Value of pre-charge period: A[7:4] Phase 2 period of up to 15 DCLK, A[3:0] Phase 1 period of up to 15 DCLK
	write_c(0x20);        // Set  Memory  Addressing  Mode
	write_c(0x02);        // Value of memory addressing: A[1:0]: 00b Horizontal Addressing, 01b Vertical Addressing, 10b Page Addressing, 11b Invalid
	write_c(0xdb);        // VCOM deselect  level  mode
	write_c(0x30);        // Value of VCOM deselect: A[6:4]: 000b 0.65 x Vcc, 010b 0.77 x Vcc, 011b 0.83 x Vcc
	write_c(0xad);        // master configuration
	write_c(0x00);        // Value of master: A[4]: 0 external Iref, 1 internal Iref    
	write_c(0xa4);        // out follows RAM  content
	write_c(0xa6);        // set  normal display
	write_c(0xaf);        // display on
}

void OledPrint(uint8_t* data)
{
	write_d(pgm_read_byte(&data));
}

int main(void)
{
	uint8_t data PROGMEM = 0xa;
	data = 0xe;
	
	USART_Init ( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	ExernalMemoryInit();
	
	OledInit();
	OledPrint(data);
	
    while(1)
    {
        printf("Hello!");
		
		_delay_ms(100);
    }
}