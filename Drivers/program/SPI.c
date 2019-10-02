#include "SPI.h"

void SPI_MasterInit(void)
{
/* Set MOSI ~SS and SCK output, all others input */
DDRD = (1<<DDB5)|(1<<DDB7) | (1<<DDB4);
/* Enable SPI, Master, set clock rate fck/16 */
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}


char SPI_MasterTransmit(char sendData)
{
    char receiveData;
    /* Start transmission */
    SPDR = sendData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    receiveData = SPDR;
}