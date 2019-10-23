/*
 * SPI.h
 *
 * Created: 02.10.2019 13:46:17
 *  Author: felixas
 */ 


#ifndef SPI_H_
#define SPI_H_


void SPI_MasterInit(void);
void SPI_write(char);
char SPI_read(void);
char SPI_MasterTransceive(char sendData);
void SPI_SlaveSelect(void);
void SPI_SlaveDeselect(void);


#endif /* SPI_H_ */