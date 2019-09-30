#ifndef ADC_H
#define ADC_H


#include <stdio.h>

#define ADC_ADDRESS 0x1400
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
//#define JOYSTICK_THRESOLD 25 //Filter out fluctuations when joystick is at the center

typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} channel_t;		// data type to select adc channels
/*typedef enum {CENTER, LEFT, RIGHT, DOWN, UP} joystick_direction_t;		// Joystick direction



typedef struct joystick_data
{
	uint8_t x_offset;
	uint8_t y_offset;
	uint8_t menuCounter;
}joystick_data_t;
*/

void adcInit( void );


#endif // ADC_H