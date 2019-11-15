/*
 * game.h
 *
 * Created: 13.11.2019 13:23:14
 *  Author: felixas
 */ 


#ifndef GAME_H_
#define GAME_H_

typedef enum{
	GAMEOVER, GAMESTART, GAMESTOP, IDLE, INIT, RUNGAME
}gameStatus_t;

gameStatus_t gameStatus;



#endif /* GAME_H_ */