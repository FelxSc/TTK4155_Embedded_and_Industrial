/*
 * game.c
 *
 * Created: 13.11.2019 11:49:08
 *  Author: felixas
 */

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>

#include "Interrupt.h"
#include "game.h"
#include "SRAM.h"
#include "JOYSTICK.h"
#include "CAN.h"
#include "bitMacro.h"
#include "EEPROM.h"
#include "OLED.h"

uint8_t leftButtonFlag = 0;


void gameButtonInit(void)
{
	// PB3 as input
	DDRB &= ~(1 << PB2);

}

void game(void){
	
	uint16_t highscore;
	uint8_t scoreHigh, scoreLow, highscoreLow, highscoreHigh;
		
	do{
		switch(state){
			case START:
				OLEDGotoPosition(2,2);
				
				OLEDPrintf("Left button: Start game");
				OLEDGotoPosition(3,2);
				OLEDPrintf("Right button: Cancel");
				printf("push RIGHT touch button to start the game\n\r");
			
				gameStatus = GAMESTOP;
				while(!test_bit(PINB,PINB2) & !test_bit(PINB,PINB3)){}	// if gameButton is pressed - start the game
				if(test_bit(PINB,PINB2))
					state = INIT;
				else
					state = START;
				break;
			case INIT:
				printf("INIT\n\r");		
				gameStatus = GAMESTART;
				gameTimer = 0;
				state = RUNGAME;
				break;
		
			case RUNGAME:
				printf("RUNGAME\n\r");
				enableGameTimer();
				while(gameStatus != GAMEOVER)
				{				
					// Test Left Touch button to activate solenoid
					if(test_bit(PINB,PINB3))
					{	slider_data.leftbutton = 1; /*printf("button on\n\r");*/	}
					else
					{	slider_data.leftbutton = 0;  /*printf("Button off\n\r");*/	}
			
					if(CAN_interrupt == 1){
						game_data = handleCANreceiveInterrupt();
						gameStatus = game_data.msg[3];
					}
			
					if(Timer1_interrupt)
						SendTimer1CANmsg();
					//printf("gametimer: %d",gameTimer);
				
					_delay_ms(50);
								
				}
				state = GAMEOVER;
				disableGameTimer();
			
				break;
			case GAMEOVER:
				printf("GAMEOVER");
				printf("GameTimer: %d", gameTimer);
			
				scoreLow = gameTimer;
				scoreHigh = gameTimer >> 8;
			
			
				printf("ScoreLow = %d\n\r", scoreLow);
				printf("ScoreHigh = %d\n\r", scoreHigh);
			
				// save the highscore to SRAM.
				highscoreLow = EEPROM_read(0);
				highscoreHigh = EEPROM_read(1);
				highscore = ((uint16_t)highscoreHigh << 8) | highscoreLow;

				if( gameTimer > highscore ){
					EEPROM_write(0, scoreLow);
					EEPROM_write(1, scoreHigh);
				}
				printf("highscore: %d", highscore);
			
				// show HIGHSCORE and CURRENT score to OLED. - And an appropriate message
				state = START;
				break;
			default: printf("I don't know what state I'm in");
		}
	}while(state != START);
}