/*
 * JOYSTICK.h
 *
 * Created: 28.09.2019 10:51:22
 *  Author: oddiha
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdio.h>


#define JOYSTICK_THRESOLD 25 //Filter out fluctuations when joystick is at the center

typedef enum {CENTER, LEFT, RIGHT, DOWN, UP} joystick_direction_t;		// Joystick direction


typedef struct joystick_data
{
	uint8_t x_offset;
	uint8_t y_offset;
	uint8_t x_position;
	uint8_t y_position;
	joystick_direction_t joystickPosition;
	uint8_t menuCounter;
}joystick_data_t;

typedef struct slider_data
{
	uint8_t rightslider;
	uint8_t leftslider;
	uint8_t rightbutton;
	uint8_t leftbutton;
}slider_data_t;


void sliderDriver(void);
joystick_direction_t get_joystick_direction(uint8_t joyst_coord_X, uint8_t joyst_coord_Y);
void joystickCalibrate(void);
void joystickDriver(void);

slider_data_t slider_data;
joystick_data_t joystickCalibration;
joystick_data_t joystick_data;




#endif /* JOYSTICK_H_ */