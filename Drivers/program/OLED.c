/*
 * OLEDTest.c
 *
 * Created: 18.09.2019 17:08:55
 *  Author: oddiha
 */ 
#ifndef F_CPU
#define F_CPU 4915200UL // 4.9152 Mhz

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
#include "OLEDStrings.h"
#include "JOYSTICK.h"
#include "bitMacro.h"
#include "Interrupt.h"
#include "SRAM.h"
#include "Menu.h"
#include "Music.h"


volatile char* extOledCmd = OLED_COMMAND_ADDRESS;
volatile char* extOledData = OLED_DATA_ADDRESS;
volatile OLED_t OLEDpos = {0, 0, 64, 62};
volatile uint8_t FONTSIZE = 8;


//char* OLEDString[16];
//#define OLEDFrame ((uint8_t(*)[128]) (SRAM_FIRST_ADDRESS))


void write_c(uint8_t command)
{
	extOledCmd[0] = command;
}

void write_d(uint8_t data)
{
	extOledData[0] = data;
}

/*void write_s(uint8_t data)
{
	OLEDframe[OLEDpos.line][OLEDpos.column] = data;
	OLEDpos.column++;
	if (OLEDpos.column > 128)
	{
		OLEDpos.column = 0;
	}
}*/


/*char* get_OLEDString(uint8_t OLEDStringID)
{
	strcpy_P(OLEDString, (PGM_P)pgm_read_byte(&(OLEDStringTable[OLEDStringID])));
	return OLEDString;
}*/



void OLEDGotoLine(unsigned char line)
{
	if (line < MAX_PAGES)
	{
		write_c(0xB0 + line); // B0 - B7 <- page address  --  8 lines per page
		write_c(0x00);
		write_c(0x10);
		
		OLEDpos.line = line;
	}
	
}

void OLEDGotoColumn(unsigned char column)
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


void OLEDGotoPosition(unsigned char line, unsigned char column)
{
	OLEDGotoLine(line);
	OLEDGotoColumn(column);
}


void OLEDHome(void)
{
	OLEDGotoLine(0x00);
	OLEDGotoColumn(0x00);
}

void OLEDClearLine(unsigned char line)
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

void OLEDprintf(char* data, ...)// not changed from uint8_t
{
	va_list args;
	va_start(args, data);
	vfprintf(&mystdout, &data, args);
	va_end(args);
}

void OLEDinvPrintf(char* data, ...)// not changed from uint8_t
{
	va_list args;
	va_start(args, data);
	vfprintf(&myinvstdout, &data, args);
	va_end(args);
}


int OLEDinvertedPrint(unsigned char data)	// not changed from uint8_t
{
	for (int i=0; i < FONTSIZE; i++) {
		switch (FONTSIZE)
		{
			case 4: write_d(~pgm_read_byte(&(font4[data - ' '][i]))); break;
			case 5: write_d(~pgm_read_byte(&(font5[data - ' '][i]))); break;
			case 8: write_d(~pgm_read_byte(&(font8[data - ' '][i]))); break;
		}
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



int OLEDPrint(unsigned char data)// not changed from uint8_t
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
	return 0;
}



void OLEDContrast(void)
{
		
	OLEDGotoPosition(6,12);
	fprintf(OLED_p, "0%%");
	
	OLEDGotoPosition(6,84);
	fprintf(OLED_p, "100%%");

	
	OLEDcreateBar();
	
	do{
		joystickDriver();
		
		if(joystick_data.joystickPosition == RIGHT && OLEDpos.column < 115)
		{
			if(OLEDpos.column < 11)
				{ OLEDpos.column = 11; OLEDGotoColumn(OLEDpos.column); }			
			
			OLEDpos.brightness++;
			
			write_c(0x81);
			write_c(OLEDpos.brightness);
			write_d(0b11111111);
			
			OLEDpos.column++;
			OLEDGotoColumn(OLEDpos.column);
		}
		else if(joystick_data.joystickPosition == LEFT && OLEDpos.column > 13 )
		{
			if(OLEDpos.column > 114)
				{ OLEDpos.column = 113; OLEDGotoColumn(OLEDpos.column); }
				
			OLEDpos.brightness--;
			
			write_c(0x81);
			write_c(OLEDpos.brightness);
			write_d(0b11000011);
						
			OLEDpos.column--;
			OLEDGotoColumn(OLEDpos.column);
		}
		
		_delay_ms(50);
	}while(joystickButtonInterrupt == 0);
	
	joystickButtonInterrupt = 0;
	OLEDpos.LastBrightnessPos = OLEDpos.column;
	
}

void setFontSize(uint8_t fontsize)
{
	FONTSIZE = fontsize;
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
	fprintf(OLED_p, "SCORE");
	
	OLEDGotoPosition(4,60);
	fprintf(OLED_p, "%d", gameTimer);
}

void OLEDGameOver(uint16_t highscore)
{
	OLEDClearAll();
	
	OLEDGotoPosition(0, 32); // Print 'Game Over' in the middle of the screen
	fprintf(OLED_p, "GAMEOVER");
	
	OLEDGotoPosition(2, 24);
	fprintf(OLED_p, "YOUR SCORE");
	
	OLEDGotoPosition(3,56); // Calculate exact position??? Or just approximately?
	fprintf(OLED_p, "%d", gameTimer);
	
	OLEDGotoPosition(5,28);
	fprintf(OLED_p, "HIGHSCORE");
	
	OLEDGotoPosition(6,56);
	fprintf(OLED_p, "%d", highscore);
}

void OLEDNewHighscore(void)
{
	uint8_t highscoreTimer = 0;
	uint8_t toggleBrigthness = 0;
	
	OLEDClearAll();
	
	OLEDGotoPosition(2, 52);
	fprintf(OLED_p, "NEW");

	OLEDGotoPosition(3, 28);
	fprintf(OLED_p, "HIGHSCORE");
	
	OLEDGotoPosition(5, 56);
	fprintf(OLED_p, "%d", gameTimer);
	
	// CHANGE THIS, TOO MUCH MEMORY
	while (highscoreTimer < 50)
	{
		highscoreTimer++;
		if (toggleBrigthness == 0)
		{
			write_c(0x81);
			write_c(0x08);
			toggleBrigthness = 1;
		}
		else
		{
			write_c(0x81);
			write_c(0x80);
			toggleBrigthness = 0;
		}
		
		_delay_ms(300);
	}
	
	write_c(0x81);
	write_c(OLEDpos.LastBrightnessPos);
	
}

void OLEDAfterGame(void)
{
	OLEDGotoPosition(7, 0);
	fprintf(OLED_p, "<-Return");
	
	OLEDGotoPosition(7, 72);
	fprintf(OLED_p, "Again->");
}

void OLEDStartGame(void)
{
	OLEDClearAll();
	setBuzzerFrequency((NOTE_C6));
		
	for (int i = 0; i<3; i++)
	{
		enableBuzzer();
		OLEDGotoPosition(3, 60);
		fprintf(OLED_p, "%d", (3-i));
		_delay_ms(800);
		disableBuzzer();
		_delay_ms(200);
	
	}
	setBuzzerFrequency(NOTE_C7);
	enableBuzzer();
	OLEDGotoPosition(3, 52);
	fprintf(OLED_p, "GO!");
	_delay_ms(1000);
	disableBuzzer();
}

void OLEDShowHighscore(void)
{
	
	uint16_t highscore1, highscore2, highscore3;
	
	// Check if new score is new highscore
	highscore1 = ReadHighscore(0);
	highscore2 = ReadHighscore(2);
	highscore3 = ReadHighscore(4);
	do{
			OLEDGotoPosition(2,0);
			fprintf(OLED_p, "1. %d", highscore1);
			OLEDGotoPosition(3,0);
			fprintf(OLED_p, "2. %d", highscore2);
			OLEDGotoPosition(4,0);
			fprintf(OLED_p, "3. %d", highscore3);
	}while(joystickButtonInterrupt == 0);

}

#endif