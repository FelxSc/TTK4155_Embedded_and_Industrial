/*
 * solenoid.c
 *
 * Created: 13.11.2019 15:26:06
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#include <avr/io.h>

void solenoidButtonInit(void)
{
	// PB3 as input
	DDRB &= ~(1 << PB3);
}