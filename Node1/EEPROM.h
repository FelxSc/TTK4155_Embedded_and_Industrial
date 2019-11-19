/*
 * EEPROM.h
 *
 * Created: 15.11.2019 18:55:15
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

void EEPROM_write(uint8_t Address, uint8_t Data);
uint8_t EEPROM_read(uint8_t Address);
void EEPROM_reset(uint8_t num);



#endif /* EEPROM_H_ */