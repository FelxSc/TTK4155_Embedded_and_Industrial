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


void game(void){
	
	uint16_t highscore;
	uint8_t scoreHigh, scoreLow, highscoreLow, highscoreHigh;
	
		
	
	switch(state){
		case IDLE:
			printf("IDLE\n\r");
			gameStatus = GAMESTOP;
			
						
			if(joystickButtonInterrupt == 1)
				{ joystickButtonInterrupt = 0; state = INIT; }
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
				{	slider_data.leftbutton = 1; printf("button on\n\r");
					Timer1_interrupt = 1;
				}
				else
				{	slider_data.leftbutton = 0; printf("Button off\n\r");	}
			
				if(CAN_interrupt == 1){
					game_data = handleCANreceiveInterrupt();
					gameStatus = game_data.msg[4];
				}
			
				if(Timer1_interrupt)
					SendTimer1CANmsg();   // PUT THIS BACK IN
				printf("gametimer: %d",gameTimer);
				
				_delay_ms(10);
								
			}
			state = GAMEOVER;
			disableGameTimer();
			
			break;
		case GAMEOVER:
			printf("GAMEOVER");
			printf("GameTimer: %d", gameTimer);
			
			scoreLow = gameTimer;
			scoreHigh = gameTimer >> 8;
			
			gameTimer = 0;
			printf("ScoreLow = %d\n\r", scoreLow);
			printf("ScoreHigh = %d\n\r", scoreHigh);
			
			// save the highscore to SRAM.
			highscoreLow = SRAM_read(0x1);
			highscoreHigh = SRAM_read(0x2);
			highscore = ((uint16_t)highscoreHigh << 8) | highscoreLow;

			if( gameTimer > highscore ){
				SRAM_write(0x01, scoreLow);
				SRAM_write(0x02, scoreHigh);
			}
			printf("highscore: %d", highscore);
			
			// show HIGHSCORE and CURRENT score to OLED. - And an appropriate message
			state = IDLE;
			break;
		default: printf("I don't know what state I'm in");
	}
}