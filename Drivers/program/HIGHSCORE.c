/*
 * HIGHSCORE.c
 *
 * Created: 18.11.2019 22:35:22
 *  Author: felixas
 */ 

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

#include "HIGHSCORE.h"
#include "EEPROM.h"

uint16_t ReadHighscore(uint8_t eeprom_pos)
{
	uint8_t highscoreLow, highscoreHigh;
	uint16_t highscore;
	
	highscoreLow = EEPROM_read(eeprom_pos);
	eeprom_pos++;
	highscoreHigh = EEPROM_read(eeprom_pos);
	
	return highscore = ((uint16_t)highscoreHigh << 8) | highscoreLow;
}

uint8_t saveHighscore(uint16_t newscore)
{
	uint8_t scoreHigh, scoreLow;
	uint16_t highscore1, highscore2, highscore3;
	
	// Check if new score is new highscore
	highscore1 = ReadHighscore(0);
	highscore2 = ReadHighscore(2);
	//highscore3 = ReadHighscore(4);
		
	scoreLow = newscore;
	scoreHigh = newscore >> 8;
	
	// Save new highscore	
	if( newscore > highscore1 ){
		EEPROM_write(0, scoreLow);
		EEPROM_write(1, scoreHigh);
		// save old highscore at lower memory
		EEPROM_write(2, highscore1);
		EEPROM_write(3, highscore1 >> 8);
		// save old highscore at lower memory
		EEPROM_write(4, highscore2);
		EEPROM_write(5, highscore2 >> 8);
		return 1;
	}
	return 0;
}

void resetHighscore(void)
{
	EEPROM_reset(5);
}

