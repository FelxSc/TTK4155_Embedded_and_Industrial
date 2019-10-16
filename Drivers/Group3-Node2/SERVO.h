/*
 * SERVO.h
 *
 * Created: 16.10.2019 15:37:45
 *  Author: oddiha
 */ 


#ifndef SERVO_H_
#define SERVO_H_

void ServoTimer1Init( void );
void setDutyCycle( uint8_t count );
uint8_t calculateDutyCycleCounter( uint8_t number );






#endif /* SERVO_H_ */