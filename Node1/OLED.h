/*
 * OLED.h
 *
 * Created: 18.09.2019 17:08:55
 *  Author: Odd Inge Halsos & Felix Sch�pe
 */ 

#ifndef OLED_H
#define OLED_H


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

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200
#define MAX_PAGES 0x8
#define MAX_COLUMNS 0x80
#define MAX_ROWS 0x40


void OLEDInit(void);
void OLEDReset(void);
void OLEDHome(void);
void OLEDGotoLine(uint8_t line);
void OLEDGotoColumn(uint8_t column);
void OLEDClearLine(uint8_t line);
void OLEDClearAll(void);
void OLEDClearColumn(void);
int OLEDPrint(unsigned char data);
int OLEDinvertedPrint(unsigned char data);
void OLEDprintf(char* data, ...);
void OLEDinvPrintf(char* data, ...);
void OLEDGotoPosition(uint8_t line, uint8_t column);
void OLEDprintArrowRight(void);
void OLEDprintArrowLeft(void);
void OLEDcreateBar(void);
void OLEDbrightness(void);
void OLEDScoreCounter(void);
void OLEDGameOver(uint16_t highscore);
void OLEDNewHighscore(void);
void OLEDAfterGame(void);
void OLEDStartGame(void);
void OLEDShowHighscore(void);




void setFontSize(uint8_t fontsize);
void write_c(uint8_t command);		// write command to OLED
void write_d(unsigned char data);	// write data to OLED

typedef struct OLED
{
	uint8_t line;
	uint8_t column;
	uint8_t brightness;
	uint8_t LastBrightnessPos;
}OLED_t;

//Set up for OLEDPrintf
static FILE mystdout = FDEV_SETUP_STREAM(OLEDPrint, NULL, _FDEV_SETUP_WRITE);
#define OLED_p (&mystdout)

static FILE myinvstdout = FDEV_SETUP_STREAM(OLEDinvertedPrint, NULL, _FDEV_SETUP_WRITE);
#define OLED_invp (&myinvstdout)



#endif // OLED_H