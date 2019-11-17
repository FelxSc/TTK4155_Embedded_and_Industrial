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
#include "bitMacro.h"
#include "Interrupt.h"


static FILE mystdout = FDEV_SETUP_STREAM(OLEDPrint, NULL, _FDEV_SETUP_WRITE);				
static FILE myinvstdout = FDEV_SETUP_STREAM(OLEDinvertedPrint, NULL, _FDEV_SETUP_WRITE);

volatile uint8_t* extOledCmd = (uint8_t*) OLED_COMMAND_ADDRESS;
volatile uint8_t* extOledData = (uint8_t*) OLED_DATA_ADDRESS;

extern OLED_t OLEDpos = {0,0, 64, 62};

void write_c(uint8_t command)
{
	*extOledCmd = command;
}

void write_d(uint8_t data)
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
		
		OLEDpos.line = line;
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
		
		OLEDpos.column = column;
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
	va_start(args, data);
	vfprintf(&mystdout, data, args);
	va_end(args);
	
	// take the length of the string and loop through the columns to get the current column
/*	for (int i=0; i<length*FONTSIZE;i++)
	{
		OLEDpos.column++;
		if (OLEDpos.column == 128)
		{
			OLEDpos.column = 0;
		}
	}*/
}

void OLEDinvPrintf(char* data, ...)
{
	va_list args;
	va_start(args, data);
	vfprintf(&myinvstdout, data, args);
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


int OLEDinvertedPrint(unsigned char data)
{
	//uint8_t printChar = c-32;
	
	for (int i=0; i < FONTSIZE; i++) {
		//Write_d(~pgm_read_word(&font8[printChar][i]));
		switch (FONTSIZE)
		{
			case 4: write_d(~pgm_read_byte(&(font4[data - ' '][i]))); break;
			case 5: write_d(~pgm_read_byte(&(font5[data - ' '][i]))); break;
			case 8: write_d(~pgm_read_byte(&(font8[data - ' '][i]))); break;
		}
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
	write_c(0x40);        // Value of contrast
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



void OLEDPrint(uint8_t data)
{
	for (int i=0; i<FONTSIZE; i++)
	{
		switch (FONTSIZE)
		{
		case 4: write_d(pgm_read_byte(&(font4[data - ' '][i]))); break;
		case 5: write_d(pgm_read_byte(&(font5[data - ' '][i]))); break;
		case 8: write_d(pgm_read_byte(&(font8[data - ' '][i]))); break;
		}
	}
}


void OLEDContrast(void)
{
	
	OLEDGotoPosition(3,0);
	OLEDPrintf("Use joystick!");
	OLEDGotoPosition(4,0);
	OLEDPrintf("Keep brightness?");
	OLEDGotoPosition(5,0);
	//OLEDPrintf("Press");
	OLEDcreateBar();
	
	do{
		joystickDriver();
		//printf("Column : %d", OLEDpos.column);
		if(joystick_data.joystickPosition == RIGHT && OLEDpos.column < 115)
		{
			if(OLEDpos.column < 11)
				{ OLEDpos.column = 11; OLEDGotoColumn(OLEDpos.column); }			
			
			OLEDpos.brightness++;
			
			//printf("Joystick Up = %d", joystick_data.joystickPosition);
			write_c(0x81);
			write_c(OLEDpos.brightness);
			write_d(0b11111111);
			
			OLEDpos.column++;
			OLEDGotoColumn(OLEDpos.column);
		}
		else if(joystick_data.joystickPosition == LEFT && OLEDpos.column > 13 )
		{
			//printf("Joystick Down = %d", joystick_data.joystickPosition);
			if(OLEDpos.column > 114)
				{ OLEDpos.column = 113; OLEDGotoColumn(OLEDpos.column); }
				
			OLEDpos.brightness--;
			
			write_c(0x81);
			write_c(OLEDpos.brightness);
			write_d(0b11000011);
			
			OLEDpos.column--;
			OLEDGotoColumn(OLEDpos.column);
		}
		_delay_ms(100);
	}while(!test_bit(PINB,PINB3));
	
	OLEDpos.LastBrightnessPos = OLEDpos.column;

}


void OLEDcreateBar(void)
{
	OLEDGotoPosition(7,12);
	write_d(0b11111111);
	write_d(0b11111111);
	for(int i = 0; i < (OLEDpos.LastBrightnessPos-12); i++)
	{
		write_d(0b11111111);
	}
	for(int i = 0; i < (100-(OLEDpos.LastBrightnessPos-12)); i++)
	{
		write_d(0b11000011);
	}

	write_d(0b11111111);
	write_d(0b11111111);

	OLEDGotoPosition(7,OLEDpos.LastBrightnessPos);
}


// Update this function every time the timer interrupt triggers
void OLEDScoreCounter(void)
{
	OLEDClearAll();
	OLEDGotoPosition(2, 44); // Go to approximately the middle of the screen
	OLEDPrintf("SCORE");
	OLEDGotoPosition(4,60);
	OLEDPrintf("%d", gameTimer); // Print the Score
}

void OLEDGameOver(void)
{
	
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