/*
 * IR.h
 *
 * Created: 19.10.2019 16:34:23
 *  Author: oddiha
 */ 


#ifndef ADC_H_
#define ADC_H_



#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>



void ADCinit( void );
uint16_t readADC( void );





#endif /* ADC_H_ */