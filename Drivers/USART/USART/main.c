/*
 * USART.c
 *
 * Created: 8/28/2019 7:32:57 PM
 * Author : oddingehalsos
 */ 
#define F_CPU 4915200 // 4.9152 Mhz
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/iom162.h>
#include <util/delay.h>



void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<< UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
	
}

int main(void)
{
    /* Replace with your application code */
	unsigned char data = 0x0;
	USART_Init ( MYUBRR );
	DDRB |= 0xFF;
	
	
    while (1) 
    {
		PORTB ^= 0xFF;
		data = USART_Receive();
		data = data+2; // Increase the received data value by two before it is sent back. ex. letter 'a' is increased to letter 'c'
		_delay_ms(100);
		
		USART_Transmit( data );
		for(int i  = 0; i < 20; i++){} // 20 cycles to send 10 bits
    }
}

