#ifndef USART_H
#define USART_H

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


void USART_Init( unsigned int ubrr );
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char data );


#endif // USART_H