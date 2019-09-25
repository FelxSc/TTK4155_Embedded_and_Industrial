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
#include <stdio.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#include "fonts.h"
#include "OLED.h"


static FILE mystdout = FDEV_SETUP_STREAM(OLEDPrint, NULL, _FDEV_SETUP_WRITE);				
static FILE myinvstdout = FDEV_SETUP_STREAM(OLEDinvertedPrint, NULL, _FDEV_SETUP_WRITE);

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

void write_d(char data)
{
	*extOledData = data;
}

void OLEDGotoLine(uint8_t line)
{
	if (line < MAX_PAGES)
	{
		write_c(0xB0 + line); // B0 - B7 <- page address  --  8 lines per page
		write_c(0x00);
		write_c(0x10);
	}
}

void OLEDGotoColumn(uint8_t column)
{
	if (column < MAX_COLUMNS)
	{
		uint8_t LowerColumn = (0x0F & column);
		uint8_t HigherColumn = 0x10 + ( 0x0F & (column >> 4));
		
		write_c(LowerColumn);
		write_c(HigherColumn);
	}
}

void OLEDHome(void)
{
	OLEDGotoLine(0x00);
	OLEDGotoColumn(0x00);
}

void OLEDClearLine(uint8_t line)
{
	OLEDGotoLine(line);
	for (int i=0; i<MAX_COLUMNS; i++)
	{
		write_d(0b00000000); // 0b00000000 = writes ' ' (space) to the OLED line
	}
}

void OLEDClearAll()
{
	for (int i = 0; i<MAX_PAGES; i++)
	{
		OLEDClearLine(i);
	}
}

void OLEDPrintf(char* data, ...){
	va_list args;
	va_start(args, data);
	vfprintf(&mystdout, data, args);
	va_end(args);	
}

void OLEDinvPrintf(char* data, ...){
	va_list args;
	va_start(args, data);
	vfprintf(&myinvstdout, data, args);
	va_end(args);
}


int OLEDinvertedPrint(unsigned char c){
	uint8_t printChar = c-32;
	
	for (int i=0; i < FONTSIZE; i++) {
		write_d(~pgm_read_word(&font8[printChar][i]));
		//position.col += fontSize;
		//oled_is_out_of_bounds();
	}
	
	return 0;
}


void OLEDInit()
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
	
	OLEDClearAll();
	OLEDHome();
}

/*
void OledReset()
{
	
}*/


// FIX NEEDED
void OLEDPrint(unsigned char data)
{
	for (int i=0; i<FONTSIZE; i++)
	{
		write_d(pgm_read_byte(&(font8[data - ' '][i])));
	}
}



int main(void)
{
	char data = 'a';
	
	USART_Init ( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	ExernalMemoryInit();
	
	OLEDInit();
	
	OLEDClearAll();
	OLEDGotoLine(3);
	OLEDGotoColumn(100);
	//OLEDPrintf("AWESOME SAUCE");
	OLEDinvPrintf("Poor GUY!! :-(");
    while(1)
    {
        printf("Hello!");
		
		
		/*
		write_c(0x81);
		write_c(0x50);			// contrast value
		_delay_ms(500);
		write_c(0x81);
		write_c(0x00);*/			// contrast value
		//OledPrint(data);
		//data++;
		
		
		//write_c(0xa5);
		_delay_ms(500);
		
    }
}