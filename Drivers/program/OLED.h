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
#define FONTSIZE 8

void OLEDInit(void);
void OLEDReset(void);
void OLEDHome(void);
void OLEDGotoLine(uint8_t line);
void OLEDGotoColumn(uint8_t column);
void OLEDClearLine(uint8_t line);
void OLEDClearAll(void);
void OLEDClearColumn(void);
void OLEDPrint(uint8_t);
void OLEDPrintf(char* data, ...);
void OLEDinvPrintf(char* data, ...);
void OLEDGotoPosition(uint8_t line, uint8_t column);
int OLEDinvertedPrint(unsigned char data);
void OLEDprintArrowRight(void);
void OLEDprintArrowLeft(void);
void OLEDcreateBar(void);
void OLEDContrast(void);
void OLEDScoreCounter(void);


void write_c(uint8_t command);	// write commands to OLED
void write_d(unsigned char data);		// write data to OLED

typedef struct OLED
{
	uint8_t line;
	uint8_t column;
	//uint8_t fontSize;
	uint8_t brightness;
	uint8_t LastBrightnessPos;
}OLED_t;

OLED_t OLEDpos;

#endif // OLED_H