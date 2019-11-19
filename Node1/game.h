/*
 * game.h
 *
 * Created: 13.11.2019 11:49:30
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 
#define F_CPU 4915200UL // 4.9152 Mhz


#ifndef GAME_H_
#define GAME_H_

typedef enum{
	GAMEOVER, GAMESTART, GAMESTOP, START, INIT, RUNGAME 
	}gameStatus_t; 

 gameStatus_t gameStatus;
 gameStatus_t state;


void game(void);


#endif /* GAME_H_ */