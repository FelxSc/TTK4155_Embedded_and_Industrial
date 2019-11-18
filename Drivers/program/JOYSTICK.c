/*
 * JOYSTICK.c
 *
 * Created: 28.09.2019 10:49:16
 *  Author: oddiha
 */ 

#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "ADC.h"
#include "JOYSTICK.h"



//extern joystick_data_t joystickCalibration;
//extern joystick_data_t joystick_data;

void joystickButtonInit( void )
{
	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);

	cli();		
	// Interrupt on falling edge
	MCUCR &= ~(1<<ISC10);
	MCUCR |= (1<<ISC11);

		
	// Enable Interrupt on PD3
	GICR |= (1<<INT1);
	sei();
}


void sliderDriver()
{
	slider_data.leftslider = getADCdata(CHANNEL1);
	slider_data.rightslider = getADCdata(CHANNEL2);
}


joystick_direction_t get_joystick_direction(uint8_t joyst_coord_X, uint8_t joyst_coord_Y)
{
	
	int8_t signed_joyst_coord_X, signed_joyst_coord_Y;
	uint8_t abs_signed_joyst_coord_X, abs_signed_joyst_coord_Y;
	
	signed_joyst_coord_X = joyst_coord_X - 128;
	signed_joyst_coord_Y = joyst_coord_Y - 128;
	abs_signed_joyst_coord_X = abs(signed_joyst_coord_X);
	abs_signed_joyst_coord_Y = abs(signed_joyst_coord_Y);
		
	if (abs_signed_joyst_coord_X <= JOYSTICK_THRESOLD && abs_signed_joyst_coord_Y <= JOYSTICK_THRESOLD)
		return CENTER;
	else if (signed_joyst_coord_X > abs_signed_joyst_coord_Y )
		return RIGHT;
	else if (signed_joyst_coord_X < - abs_signed_joyst_coord_Y)
		return LEFT;
	else if (signed_joyst_coord_Y > abs_signed_joyst_coord_X)
		return UP;
	else
		return DOWN;
	
}

// When joystick is centered the measured center value will be the offset
void joystickCalibrate( void )
{
	joystickCalibration.x_offset = getADCdata(CHANNEL4); //X axis
	joystickCalibration.y_offset = getADCdata(CHANNEL3); //Y axis
}

void joystickDriver()
{
	joystick_data.x_position = getADCdata(CHANNEL4); //X axis
	joystick_data.y_position = getADCdata(CHANNEL3); //Y axis
	joystick_data.joystickPosition = get_joystick_direction(joystick_data.x_position, joystick_data.y_position);
}