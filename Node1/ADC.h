/*
 * adcTest.h
 *
 * Created: 11.09.2019 16:13:05
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 

#ifndef ADC_H
#define ADC_H


#include <stdio.h>

#define ADC_ADDRESS 0x1400
#define FOSC 4915200	// Frequency Oscillator
#define BAUD 9600		
#define MYUBRR FOSC/16/BAUD-1

typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} channel_t;		// data type to select ADC channels


void adcInit( void );
void select_ADC_channel(channel_t channel);
uint8_t getADCdata(channel_t channel);


#endif // ADC_H