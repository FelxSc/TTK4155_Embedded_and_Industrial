/*
 * OLEDTest.c
 *
 * Created: 18.09.2019 17:08:55
 *  Author: oddiha
 */ 


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
#include <string.h>
#include <avr/pgmspace.h>

#include "FONTS.h"
#include "OLED.h"
#include "JOYSTICK.h"


static FILE mystdout = FDEV_SETUP_STREAM(OLEDPrint, NULL, _FDEV_SETUP_WRITE);				
static FILE myinvstdout = FDEV_SETUP_STREAM(OLEDinvertedPrint, NULL, _FDEV_SETUP_WRITE);

volatile uint8_t* extOledCmd = (uint8_t*) OLED_COMMAND_ADDRESS;
volatile uint8_t* extOledData = (uint8_t*) OLED_DATA_ADDRESS;

extern OLEDposition_t OLEDpos = {0,0};

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

//void OLEDUpdate

void OLEDGotoPosition(uint8_t line, uint8_t column)
{
	OLEDGotoLine(line);
	OLEDGotoColumn(column);
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

void OLEDPrintf(char* data, ...)
{
	va_list args;
	uint8_t length;
	va_start(args, data);
	vfprintf(&mystdout, data, args);
	length = strlen(data);
	//printf("Length = %d\n\r", length);
	va_end(args);
	
	// take the length of the string and loop through the columns to get the current column
	for (int i=0; i<length*FONTSIZE;i++)
	{
		OLEDpos.column++;
		if (OLEDpos.column == 128)
		{
			OLEDpos.column = 0;
		}
	}
}

void OLEDinvPrintf(char* data, ...)
{
	va_list args;
	//uint8_t length; // for getting the length of the string
	va_start(args, data);
	vfprintf(&myinvstdout, data, args);
	//length = strlen(data);
	va_end(args);
	
/*	// take the length of the string and loop through the columns to get the current column
	for (int i=0; i<length*FONTSIZE;i++)
	{
		OLEDpos.column++;
		if (OLEDpos.column == 128)
		{
			OLEDpos.column = 0;
		}
	}*/
}


int OLEDinvertedPrint(unsigned char c)
{
	uint8_t printChar = c-32;
	
	for (int i=0; i < FONTSIZE; i++) {
		write_d(~pgm_read_word(&font8[printChar][i]));
		//position.col += fontSize;
		//oled_is_out_of_bounds();
	}
	
	return 0;
}

void OLEDprintArrowRight(void)
{
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b01111110);
	write_d(0b00111100);
	write_d(0b00011000);	
	write_d(0b00000000);
}

void OLEDprintArrowLeft(void)
{
	write_d(0b00000000);
	write_d(0b00011000);
	write_d(0b00111100);
	write_d(0b01111110);
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b00011000);
	write_d(0b00011000);
}

void OLEDInit(void)
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


void OLEDPrint(unsigned char data)
{
	for (int i=0; i<FONTSIZE; i++)
	{
		write_d(pgm_read_byte(&(font8[data - ' '][i])));
	}
}


/*
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
		
		
		
		write_c(0x81);
		write_c(0x50);			// contrast value
		_delay_ms(500);
		write_c(0x81);
		write_c(0x00);			// contrast value
		//OledPrint(data);
		//data++;
		
		
		//write_c(0xa5);
		_delay_ms(500);
		
    }
}*/