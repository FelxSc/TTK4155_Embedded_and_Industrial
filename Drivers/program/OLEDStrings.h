/*
 * OLEDStrings.h
 *
 * Created: 18.11.2019 17:09:48
 *  Author: felixas
 */ 


#ifndef OLEDSTRINGS_H_
#define OLEDSTRINGS_H_

#include <avr/pgmspace.h>

// Strings that are used in OLED.c

const char PROGMEM keep_brightness[] = "Keep brightness?";
const char PROGMEM LB_Return[] = "LB Return";
const char PROGMEM score[] = "SCORE";
const char PROGMEM gameover[] = "GAMEOVER";
const char PROGMEM your_score[] = "YOUR SCORE";



PGM_P const PROGMEM OLEDStringTable[] =
{
	keep_brightness,
	LB_Return,
	score,
	gameover,
	your_score
};



#endif /* OLEDSTRINGS_H_ */