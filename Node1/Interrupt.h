/*
 * Interrupt.h
 *
 * Created: 13.11.2019 15:22:14
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "CAN.h"

volatile int CAN_interrupt;
volatile int joystickButtonInterrupt;
volatile int Timer1_interrupt;
volatile uint16_t gameTimer;


void buzzerTimer0Init( void );
void SendCAN( void );
void SendCANTimer1Init( void );
void enableSendCANTimer1( void );
void disableSendCANTimer1( void );
void gameTimer3Init( void );
void Timer3_second( void );
void Timer3_millisecond( void );
void enableGameTimer3( void );
void disableGameTimer3( void );
void enableGameTimer( void );
void disableGameTimer( void );
void receiveCanInterruptInit( void );
CAN_message_t handleCANreceiveInterrupt( void );



#endif /* INTERRUPT_H_ */