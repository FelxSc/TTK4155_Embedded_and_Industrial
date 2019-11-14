/*
 * solenoid.c
 *
 * Created: 13.11.2019 15:26:06
 *  Author: felixas
 */ 

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>

void solenoidButtonInit(void)
{
	// PB3 as input
	DDRB &= ~(1 << PB3);

}