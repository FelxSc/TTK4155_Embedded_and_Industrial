#include "SPI.h"
#include <avr/io.h>



void SPI_MasterInit(void)
{
	/* Set MOSI SCK and ~SS output, all others input */
	DDRB = (1<<DDB5)|(1<<DDB7) | (1<<DDB4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
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
	PORTB &= ~(1<<PB4);
}

void SPI_SlaveDeselect(void)
{
	/*Deselect slave*/
	PORTB |= (1<<PB4);
}