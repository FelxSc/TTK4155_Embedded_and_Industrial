/*
 * pid.h
 *
 * Created: 30.10.2019 18:13:38
 *  Author: felixas
 */ 


#ifndef PID_H_
#define PID_H_


#include "stdint.h"

#define SCALING_FACTOR 128

// Maximum value of variables
#define MAX_INT INT16_MAX
#define MAX_LONG INT32_MAX
#define MAX_INTEGRAL_TERM (MAX_LONG / 2)

// bool values
#define FALSE 0
#define TRUE 1

#define kP 0.7
#define kI 0.02
#define kD 0.8

typedef struct{
	int32_t kp;
	int32_t ki;
	int32_t kd;
	int32_t previousPosition, currentPosition, targetPosition, error, lastError, integral, derivative, distance, maxIntegral, maxError, maxDistance, minDistance;
}PID_t;

PID_t pid;





#endif /* PID_H_ */