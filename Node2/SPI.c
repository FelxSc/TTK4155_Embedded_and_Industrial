/*
 * SPI.c
 *
 * Created: 16.10.2019 15:37:13
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 


#include "SPI.h"
#include <avr/io.h>



void SPI_MasterInit(void)
{
	/* Set MOSI SCK and ~SS output, all others input */
	DDRB = (1<<PB2)|(1<<PB1)|(1<<PB0) | (1<<PB7);
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	PORTB |= (1<<PB7);
	//_delay_ms(1000);
}


char SPI_MasterTransceive(char sendData)
{
	char receiveData;
	
	/* Start transmission */
	SPDR = sendData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	

	receiveData = SPDR;

}


void SPI_write(char Data)
{
	/* Start transmission */
	SPDR = Data;
	/* Wait for transmission complete */
	loop_until_bit_is_set(SPSR, SPIF);
}

char SPI_read(void)
{

	SPDR = 0x01;
	/* Wait for transmission complete */
	loop_until_bit_is_set(SPSR, SPIF);
	//while(!(SPSR & (1<<SPIF)));

	return SPDR;
}




void SPI_SlaveSelect(void)
{
	/*Select slave*/
	PORTB &= ~(1<<PB7);
}

void SPI_SlaveDeselect(void)
{
	/*Deselect slave*/
	PORTB |= (1<<PB7);
}