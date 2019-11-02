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
	
	// Constant factors is multiplied by 128 because the final sum will be divided by 128
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

	
	// Current position - Right = 0, left = approx 8500
	pid.currentPosition = encoderRead();
	
	if(pid.currentPosition <= 36 & motor.direction == UP)
	pid.currentPosition = 36;
	
	
	pid.currentPosition = pid.currentPosition/36;
	
	
	if(pid.currentPosition > 1400)
		{ pid.currentPosition = 1;  motor.direction == DOWN; }
//	printf("\n\rCurrentPosition: %d\n\r",pid.currentPosition);
	
	pid.error = pid.targetPosition - pid.currentPosition;
	//printf("error: %d\n\r",pid.error);
	
	
	
	// Calculate Pterm and limit error overflow
	if(pid.error > pid.maxError)
		p_term = MAX_INT;
	else if (pid.error < -pid.maxError)
		p_term = -MAX_INT;
	else
		p_term = pid.kp*pid.error;




	// Calculate Iterm and limit integral runaway
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
	
	// calculate Dterm
	d_term = pid.kd*(pid.previousPosition - pid.currentPosition);
	
	pid.previousPosition = pid.currentPosition;
	
	pid.distance = (p_term + i_term + d_term) / SCALING_FACTOR;
		//printf("P_term: %d\n\r", p_term);
		//printf("i_term: %d\n\r", i_term);
		//printf("d_term: %d\n\r", d_term),
		//printf("distance: %d\n\r", pid.distance);
		
	// distance must be around 55 in order for the motor to move, due to friction.
	if(pid.distance < -120)
		{ motor.speed = 150; motor.direction = UP; }
	else if (pid.distance > 100)
		{ motor.speed = 150; motor.direction = DOWN; }
	else
		{
		if(pid.distance > 0)
			{ motor.speed = pid.distance*3; motor.direction = DOWN; }
		else if (pid.distance < 0)
			{ motor.speed = -pid.distance*3; motor.direction = UP; }
		else // pid.distance == 0
			motor.speed	= 0;
		}
}