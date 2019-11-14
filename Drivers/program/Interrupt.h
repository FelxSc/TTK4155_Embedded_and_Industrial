/*
 * Interrupt.h
 *
 * Created: 13.11.2019 15:22:14
 *  Author: felixas
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "CAN.h"

volatile int CAN_interrupt;
volatile int joystickButtonInterrupt;
volatile int Timer1_interrupt;
volatile uint16_t gameTimer;



void Timer1Init( void );
void Timer3Init( void );
void enableGameTimer( void );
void disableGameTimer( void );
void InterruptInit( void );
CAN_message_t handleCANreceiveInterrupt( void );
void SendTimer1CANmsg( void );





#endif /* INTERRUPT_H_ */