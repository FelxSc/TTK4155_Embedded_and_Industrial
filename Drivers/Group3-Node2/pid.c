/*
 * pid.c
 *
 * Created: 30.10.2019 18:13:23
 *  Author: felixas
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>


#include "pid.h"
#include "MOTOR.h"


void pidInit( void )
{
	pid.integral = 0;
	pid.lastError = 0;	
	
	// Multiply constant factors by 128 because the final sum will be divided by 128
	pid.kp = kP*SCALING_FACTOR;
	pid.ki = kI*SCALING_FACTOR;
	pid.kd = kD*SCALING_FACTOR;
	
	pid.maxError = MAX_INT / (pid.kp + 1);
	pid.maxIntegral = MAX_INTEGRAL_TERM / (pid.ki + 1);	
}


void pid_controller(void)
{
	int16_t p_term, d_term;
	int32_t i_term, temp;
	
	if(motor.targetPosition > 225)
		pid.targetPosition = 225;
	else
		pid.targetPosition = motor.targetPosition;

	
	pid.currentPosition = encoderRead();
	
	//???
	if(pid.currentPosition <= 36 & motor.direction == RIGHT)
	pid.currentPosition = 36;
	
	
	pid.currentPosition = pid.currentPosition/36;
	
	
	if(pid.currentPosition > 1400)
		{ pid.currentPosition = 1;  motor.direction == LEFT; }
	
	pid.error = pid.targetPosition - pid.currentPosition;
	
	
	
	// Calculate P_term and limit error overflow
	if(pid.error > pid.maxError)
		p_term = MAX_INT;
	else if (pid.error < -pid.maxError)
		p_term = -MAX_INT;
	else
		p_term = pid.kp*pid.error;



	// Calculate I_term and limit integral runaway
	temp=pid.integral+pid.error;
	if(temp > pid.maxIntegral){
		i_term = MAX_INTEGRAL_TERM;
		pid.integral = pid.maxIntegral;
	} else if ( temp < -pid.maxIntegral ){
		i_term = -MAX_INTEGRAL_TERM;
		pid.integral = -pid.maxIntegral;
	} else {
		pid.integral = temp;
		i_term = pid.ki*pid.integral;	
	}
	
	// calculate D_term
	d_term = pid.kd*(pid.previousPosition - pid.currentPosition);
	
	pid.previousPosition = pid.currentPosition;
	
	pid.distance = (p_term + i_term + d_term) / SCALING_FACTOR;

		
	// Note to self: distance must be around 55 in order for the motor to move, due to friction.
	if(pid.distance < -120)
		{ motor.speed = 150; motor.direction = RIGHT; }
	else if (pid.distance > 100)
		{ motor.speed = 150; motor.direction = LEFT; }
	else
		{
		if(pid.distance > 0)
			{ motor.speed = pid.distance*3; motor.direction = LEFT; }
		else if (pid.distance < 0)
			{ motor.speed = -pid.distance*3; motor.direction = RIGHT; }
		else // pid.distance == 0
			motor.speed	= 0;
		}
}



