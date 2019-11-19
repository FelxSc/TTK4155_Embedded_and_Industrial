/*
 * main.h
 *
 * Created: 05.10.2019 18:18:32
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#ifndef MAIN_H
#define MAIN_H

#include "CAN.h"

typedef enum{
	GAMEOVER, GAMESTART, GAMESTOP, IDLE, INIT, RUNGAME
}gameStatus_t;

gameStatus_t gameStatus;


int irTimer = 0;

void InterruptInit( void );
CAN_message_t handleCANInterrupt( void );

#endif //MAIN_H