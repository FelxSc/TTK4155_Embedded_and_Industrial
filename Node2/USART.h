/*
 * USART.h
 *
 * Created: 16.10.2019 15:37:13
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#ifndef USART_H
#define USART_H

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


void USART_Init( unsigned int ubrr );
unsigned char USART_Receive( void );
void USART_Transmit( unsigned char data );


#endif // USART_H