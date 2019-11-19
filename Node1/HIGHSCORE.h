/*
 * HIGHSCORE.h
 *
 * Created: 19.11.2019 01:00:10
 *  Author: Odd Inge Halsos & Felix Sch�pe
 */ 


#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

uint16_t ReadHighscore(uint8_t eeprom_pos);
uint8_t saveHighscore(uint16_t newscore);
void resetHighscore(void);


#endif /* HIGHSCORE_H_ */