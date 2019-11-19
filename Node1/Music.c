/*
 * Music.c
 *
 * Created: 18.11.2019 02:30:38
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#ifndef F_CPU
#define F_CPU 4915200UL // 4.9152 Mhz


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
#include <avr/eeprom.h>

#include "Music.h"
#include "Interrupt.h"
#include "main.h"



const uint8_t PROGMEM repeatZeldaRow[1] = {1};

const uint16_t PROGMEM ZeldaNote[1][43]  = {
	NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_F5,
	NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_F6,
	NOTE_E6, NOTE_C6, NOTE_A5, NOTE_A5, NOTE_D5,
	NOTE_F5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_D5,
	
	NOTE_F5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_F5,
	NOTE_D6, NOTE_D5, NOTE_F5, NOTE_D6, NOTE_E6,
	NOTE_F6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_C6,
	NOTE_A5, NOTE_A5, NOTE_D5, NOTE_F5, NOTE_G5,
	NOTE_A5, NOTE_A5, NOTE_D5
};


const uint16_t PROGMEM ZeldaTime[1][43] = {
	180, 180, 450, 180, 180,
	450, 400, 200, 180, 180,
	180, 180, 400, 300, 300,
	200, 200, 600, 300, 300,
	
	200, 200, 600, 180, 180,
	450, 180, 180, 450, 400,
	200, 180, 180, 180, 180,
	400, 300, 300, 200, 200,
	400, 300, 2300
};


const char repeatGameOfThroneRow[11] = {4,4,1,3,1,1,1,3,1,1,4};

const uint16_t PROGMEM GameOfThronesTime[11][5] = {
	{500, 500, 250, 250, 1},		// reapeat 4
	{500, 500, 250, 250, 1},		// reapeat 4
	{500, 500, 250, 250, 500},		// reapeat 1
	{500, 250, 250, 500, 1},		// reapeat 3
	{500, 250, 250, 1000, 1000},	// reapeat 1
	{1000, 250, 250, 1000, 1000},	// reapeat 1
	{250, 250, 500, 500, 1},		// reapeat 1
	{250, 250, 500, 1, 	1},			// reapeat 3
	{1000, 1000, 250, 250, 1000},	// reapeat 1
	{1000, 250, 250, 500, 1},		// reapeat 1
	{500, 250, 250, 500, 1},		// reapeat 4
};

const uint16_t PROGMEM GameOfThronesNote[11][5] = {
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_F4},
	{NOTE_G4, NOTE_C4, NOTE_E4, NOTE_F4, NOTE_F4},
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_D4},
	{NOTE_G3, NOTE_AS3,	NOTE_C4, NOTE_D4, NOTE_D4},
	
	{NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_F4},
	{NOTE_AS3, NOTE_DS4, NOTE_D4, NOTE_F4, NOTE_AS3},
	{NOTE_DS4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_C4},
	
	{NOTE_GS3, NOTE_AS3, NOTE_C4, NOTE_F3, NOTE_F3},
	{NOTE_G4, NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_G4},
	{NOTE_C4, NOTE_DS4, NOTE_F4, NOTE_D4, NOTE_D4},
	{NOTE_G3, NOTE_AS3, NOTE_C4, NOTE_D4, NOTE_D4}

};


void buzzerInit( void )
{
	DDRD |= (1 << PD5);
}

void enableBuzzer( void )
{
	// prescale 64
	TCCR0 |= (1 << CS01) | (1 << CS00); TCCR0 &= ~(1 << CS02);
	
	cli();
	// Enable TIMER0 Interrupt
	TIMSK |= (1 << OCIE0);
	// Initiate OCF0 flag as cleared
	TIFR &= ~(1 << OCF0);
	sei();
}

void disableBuzzer( void )
{
	TCCR0 &= ~(1 << CS01) | (1 << CS02) | (1 << CS00);
}


void setBuzzerPrescaler(uint16_t prescaler)
{
	switch (prescaler)
	{
		case 0: TCCR0 |= (1 << CS00); TCCR0 &= ~(1 << CS01) | (1 << CS02); break;
		case 8: TCCR0 |= (1 << CS01); TCCR0 &= ~(1 << CS00) | (1 << CS02); break;
		case 64: TCCR0 |= (1 << CS01) | (1 << CS00); TCCR0 &= ~(1 << CS02); break;
		case 256: TCCR0 |= (1 << CS02); TCCR0 &= ~(1 << CS01) | (1 << CS00); break;
		case 1024: TCCR0 |= (1 << CS02) | (1 << CS00); TCCR0 &= ~(1 << CS01); break; 
	}
}

void setBuzzerFrequency(uint16_t frequency)
{		
	uint8_t compareValue = 38400/frequency;
	if (compareValue > 254)
		compareValue = 254;
	else if (compareValue < 1)
		compareValue = 1;
	
	// Set Output Compare Value
	OCR0 = compareValue;
}

void SweepBuzzer( void )
{
	for(uint8_t freq = 1; freq < 255; freq++)
	{
		setBuzzerFrequency(freq);
		printf("%d",freq);
		_delay_ms(1000);
	}
}

void PlaySong(uint8_t songID)
{
	uint8_t row			= 0;
	uint8_t column		= 0;
	uint8_t repeat		= 0;
	uint8_t nrOfRow		= 0;	
	uint8_t nrOfColumn	= 0;
	
	
	
	
	switch (songID)
	{
	// Game Of Thrones theme song
	case GOT_ID:
			
			nrOfRow = 11;
			nrOfColumn = 5;
			enableBuzzer();
			while ((row < nrOfRow) & (joystickButtonInterrupt == 0))
			{	
				while ( (repeat < repeatGameOfThroneRow[row]) & (joystickButtonInterrupt == 0))
				{
					while ((column < nrOfColumn) & (joystickButtonInterrupt == 0))
					{
						setBuzzerFrequency(pgm_read_word(&(GameOfThronesNote[row][column])));
						millisecond(pgm_read_word(&(GameOfThronesTime[row][column])));
						column++;
					}
					column = 0;
					repeat++;
				}
				repeat = 0;
				row++;
			}
			disableBuzzer();
			break;


	// Zelda theme song
	case ZELDA_ID:
	
		nrOfRow = 1;
		nrOfColumn = 43;
	
		enableBuzzer();
		while ((row < nrOfRow) & (joystickButtonInterrupt == 0))
		{
			while (( repeat < pgm_read_word(&(repeatZeldaRow[row]))) & (joystickButtonInterrupt == 0))
			{
				while ((column < nrOfColumn) & (joystickButtonInterrupt == 0))
				{			
					setBuzzerFrequency(pgm_read_word(&(ZeldaNote[row][column])));
					millisecond(pgm_read_word(&(ZeldaTime[row][column])));
					column++;
				}
				column = 0;
				repeat++;
			}
			repeat = 0;
			row++;
		}
		disableBuzzer();
		break;
	}
	
	// Push joystick button to cancel theme song and return to menu
	if(joystickButtonInterrupt == 1)
		joystickButtonInterrupt == 0;
}

// Delay for X milliseconds
void millisecond(uint16_t number)
{
	while(number--){
		_delay_ms(1);
	}
}

// Play lousy melody for game without new highscore
void loserSound(void)
{
	enableBuzzer();
	for(int i = 3000; i > 1500; i -=10)
	{
		setBuzzerFrequency(i);
	}
	setBuzzerFrequency(152);
	_delay_ms(1000);
	disableBuzzer();
}

#endif