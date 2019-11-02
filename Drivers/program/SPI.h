/*
 * SPI.h
 *
 * Created: 02.10.2019 13:46:17
 *  Author: felixas
 */ 



#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdio.h>

void SPI_MasterInit(void);
void SPI_write(uint8_t);
uint8_t SPI_read(void);
uint8_t SPI_MasterTransceive(uint8_t sendData);
void SPI_SlaveSelect(void);
void SPI_SlaveDeselect(void);


#endif /* SPI_H_ */