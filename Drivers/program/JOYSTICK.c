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


void sliderDriver()
{
	uint8_t leftSliderData, rightSliderData;
	leftSliderData = getADCdata(CHANNEL1);	//Left slider
	printf("\n\rLeft slider: %d \n\r", leftSliderData);
	
	
	rightSliderData = getADCdata(CHANNEL2);	// Right slider
	printf("Right slider: %d \n\r", rightSliderData);
}

joystick_direction_t get_joystick_direction(uint8_t joyst_coord_X, uint8_t joyst_coord_Y)
{
	
	int8_t signed_joyst_coord_X, signed_joyst_coord_Y;
	uint8_t abs_signed_joyst_coord_X, abs_signed_joyst_coord_Y;
	
	signed_joyst_coord_X = joyst_coord_X - 128;
	signed_joyst_coord_Y = joyst_coord_Y - 128;
	abs_signed_joyst_coord_X = abs(signed_joyst_coord_X);
	abs_signed_joyst_coord_Y = abs(signed_joyst_coord_Y);
	
	/*printf("signed X = %d\n\r", signed_joyst_coord_X);
	printf("signed Y = %d\n\r", signed_joyst_coord_Y);
	printf("abs signed X = %d\n\r", abs_signed_joyst_coord_X);
	printf("abs signed Y = %d\n\r", abs_signed_joyst_coord_Y);
	_delay_ms(4000);*/
	
		
	if (abs_signed_joyst_coord_X <= JOYSTICK_THRESOLD && abs_signed_joyst_coord_Y <= JOYSTICK_THRESOLD){
		return CENTER;
	}
	
	if (signed_joyst_coord_X > abs_signed_joyst_coord_Y )
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
	joystickCalibration.x_offset = getADCdata(CHANNEL4); //X axis on channel 4
	joystickCalibration.y_offset = getADCdata(CHANNEL3); //Y axis on channel 3
}

void joystickDriver()
{
	joystick_data.x_position = getADCdata(CHANNEL4); //X axis on channel 4
	joystick_data.y_position = getADCdata(CHANNEL3); //Y axis on channel 3
	
	
	//joystick_direction_t joyst_direction; //The value should be: CENTER/LEFT/RIGHT/DOWN/UP
	joystick_data.joystickPosition = get_joystick_direction(joystick_data.x_position, joystick_data.y_position);
	
	
	switch(joystick_data.joystickPosition){
		case CENTER:
		printf("CENTER\n\r");
		break;
		case LEFT:
		printf("LEFT\n\r");
		break;
		case RIGHT:
		printf("RIGHT\n\r");
		break;
		case DOWN:
		printf("DOWN\n\r");
		break;
		case UP:
		printf("UP\n\r");
		break;
		default:
		printf("The function get_joystick_direction is not returning one of these: CENTER/LEFT/RIGHT/DOWN/UP\n\r");
		
	}
	printf("Joystick X = %d \n\r", joystick_data.x_position);
	printf("Joystick Y = %d \n\r", joystick_data.y_position);
}