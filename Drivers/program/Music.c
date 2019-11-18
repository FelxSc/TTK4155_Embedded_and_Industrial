/*
 * Music.c
 *
 * Created: 18.11.2019 02:30:38
 *  Author: felixas
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

const uint16_t PROGMEM ZeldaNote[43]  = {
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


const uint16_t PROGMEM ZeldaTime[43] = {
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

// Timer0 pre-scaled: 256 - Min freq (OCR0 = 255): 75.29 Hz - Max freq (OCR0 = 1): 19200 kHz
void setBuzzerFrequency(uint16_t frequency)
{
			printf("Frequency = %d - ", frequency);
			
	uint8_t compareValue = 38400/frequency;
	if (compareValue > 254)
		compareValue = 254;
	else if (compareValue < 1)
		compareValue = 1;
	
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
	uint16_t songtimer = 0;
	uint8_t number = 0;
	uint16_t limit = 0;
	switch (songID)
	{
	/*case GameOfThrones_ID:
		for ()
		{
		}
		break;*/
	case Zelda_ID:
		disableGameTimer3();
		Timer3_millisecond();
		enableGameTimer3();
		enableBuzzer();
		//for (uint8_t i = 0; i < 43; i++)
		while (number != 43)
		{
			printf("Index: %d - ", number);
			
			setBuzzerFrequency(pgm_read_word(&(ZeldaNote[number])));
			
			limit = pgm_read_word(&(ZeldaTime[number]));
			printf("Time = %d\n\r", limit);
			
			
			millisecond(limit);
		number++;
		}
		number = 0;
		limit = 0;
		disableBuzzer();
		break;
	}
}

void millisecond(uint16_t number)
{
	while(number--){
		_delay_ms(1);
	}
}

/*
void GameOfThrones( void )
{
	for(int i=0; i<4; i++)
	{
		setBuzzerFrequency( NOTE_G4);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_C4);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_DS4);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_F4);
		_delay_ms(250);
		// noTone??
	}
	for(int i=0; i<4; i++)
	{
		setBuzzerFrequency( NOTE_G4);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_C4);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_E4);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_F4);
		_delay_ms(250);
		// noTone??
	}
	setBuzzerFrequency( NOTE_G4);
	_delay_ms(500);
	// noTone??
	setBuzzerFrequency( NOTE_C4);
	_delay_ms(500);
	setBuzzerFrequency( NOTE_DS4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_F4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_D4);
	_delay_ms(500);
	// noTone??
	for(int i=0; i<3; i++)
	{
		setBuzzerFrequency( NOTE_G3);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_AS3);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_C4);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_D4);
		_delay_ms(500);
		// noTone??
	}//
	setBuzzerFrequency( NOTE_G3);
	_delay_ms(500);
	// noTone??
	setBuzzerFrequency( NOTE_AS3);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_C4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_D4);
	_delay_ms(1000);
	// noTone??
	
	setBuzzerFrequency( NOTE_F4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_AS3);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_DS4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_D4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_F4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_AS3);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_DS4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_D4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_C4);
	_delay_ms(500);
	// noTone??
	for(int i=0; i<3; i++)
	{
		setBuzzerFrequency( NOTE_GS3);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_AS3);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_C4);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_F3);
		_delay_ms(500);
		// noTone??
	}
	setBuzzerFrequency( NOTE_G4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_C4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_DS4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_F4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_G4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_C4);
	_delay_ms(1000);
	// noTone??
	setBuzzerFrequency( NOTE_DS4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_F4);
	_delay_ms(250);
	// noTone??
	setBuzzerFrequency( NOTE_D4);
	_delay_ms(500);
	// noTone??
	for(int i=0; i<4; i++)
	{
		setBuzzerFrequency( NOTE_G3);
		_delay_ms(500);
		// noTone??
		setBuzzerFrequency( NOTE_AS3);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_C4);
		_delay_ms(250);
		// noTone??
		setBuzzerFrequency( NOTE_D4);
		_delay_ms(500);
		// noTone??
	}
	disableBuzzer();
}
*/
void ZeldaSong( void )  //here is where all the notes for the song are played.
{
	setBuzzerFrequency( NOTE_D5 );  //   setBuzzerFrequency(( -PIN OF SPEAKER-, -THE NOTE WANTING TO BE PLAYED-, -DURATION OF THE NOTE IN MILISECONDS- )
	_delay_ms(180);
	
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_D6 );
	_delay_ms(450);
	
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_D6 );
	_delay_ms(450);
	
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(400);
	setBuzzerFrequency( NOTE_F6 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_F6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_C6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(400);
	
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_G5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(600);
	
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_G5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_E5 );
	_delay_ms(600);
	
	
	
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_D6 );
	_delay_ms(450);
	
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_D6 );
	_delay_ms(450);
	
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(400);
	setBuzzerFrequency( NOTE_F6 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_F6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_E6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_C6 );
	_delay_ms(180);
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(400);
	
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_F5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_G5 );
	_delay_ms(200);
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(400);
	setBuzzerFrequency( NOTE_A5 );
	_delay_ms(300);
	setBuzzerFrequency( NOTE_D5 );
	_delay_ms(2300);
	
	disableBuzzer();	
}

#endif