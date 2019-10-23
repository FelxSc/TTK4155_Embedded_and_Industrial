/*
 * MOTOR.h
 *
 * Created: 23.10.2019 15:01:18
 *  Author: oddiha
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>


typedef enum{CENTER, LEFT, RIGHT, DOWN, UP} motor_direction_t;

typedef struct{
	uint8_t speed;
	motor_direction_t direction;
	}motor_t;
	
	motor_t motor;



void motorDriverInit( void );
uint16_t encoderRead( void );
void setMotorSpeed(void);
void setMotorDirection( void );
void motorDriver( void );




#endif /* MOTOR_H_ */