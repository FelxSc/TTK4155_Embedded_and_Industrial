/*
 * EEPROM.c
 *
 * Created: 15.11.2019 18:53:15
 *  Author: Odd Inge Halsos & Felix Schöpe
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

#include "EEPROM.h"


void EEPROM_write(uint8_t Address, uint8_t Data)
{	
	/* Disable interrupts */
	cli();
	
	/* Wait for completion of previous write */
	loop_until_bit_is_clear(EECR,EEWE);
	
	/* Set up address and data registers */
	EEARH &= ~(1 << 0);
	EEARL = Address;
	
	// Write data to EEPROM
	EEDR = Data;
	
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
	
	/* Enable interrupts */
	sei();
}

uint8_t EEPROM_read(uint8_t Address)
{
	uint8_t data;
	
	/* Disable interrupts */
	cli();
	
	/* Wait for completion of previous write */
	loop_until_bit_is_clear(EECR,EEWE);
	
	/* Set up address register */
	EEARH &= ~(1 << 0);
	EEARL = Address;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	data = EEDR;
	
	/* Enable interrupts */
	sei();
	
	/* Return data from data register */
	return data;
	
}

void EEPROM_reset(uint8_t num)
{
	for(uint8_t i=0; i<num; i++)
	{
		EEPROM_write(i,0);
	}
}