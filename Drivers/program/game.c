/*
 * game.c
 *
 * Created: 13.11.2019 11:49:08
 *  Author: felixas
 */
#define F_CPU 4915200UL // 4.9152 Mhz


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
				gameStatus = GAMESTOP;
				state = INIT;
				break;
			case INIT:
				//printf("INIT\n\r");		
				gameStatus = GAMESTART;
				gameTimer = 0;
				OLEDStartGame();
				state = RUNGAME;
				enableSendCANTimer1();
				Timer3_second();
				enableGameTimer3();
				break;
		
			case RUNGAME:
				printf("RUNGAME\n\r");
				
				while(gameStatus != GAMEOVER)
				{				
					// Test Left Touch button to activate solenoid
					if(test_bit(PINB,PINB3))
					{	slider_data.leftbutton = 1; /*printf("button on\n\r");*/ }//_delay_ms(50);	}
					else
					{	slider_data.leftbutton = 0;  /*printf("Button off\n\r");*/	}
			
					if(CAN_interrupt == 1){
						
						game_data = handleCANreceiveInterrupt();
						gameStatus = game_data.msg[3];
					}
			
					if(Timer1_interrupt)
						{ SendCAN(); OLEDScoreCounter();}
					//printf("gametimer: %d",gameTimer);
				
					_delay_ms(20);
								
				}
				state = GAMEOVER;
			
				break;
			case GAMEOVER:
				enableSendCANTimer1();
				disableGameTimer3();
				//printf("GAMEOVER");
				//printf("GameTimer: %d", gameTimer);
			
				scoreLow = gameTimer;
				scoreHigh = gameTimer >> 8;
			
			
				//printf("ScoreLow = %d\n\r", scoreLow);
				//printf("ScoreHigh = %d\n\r", scoreHigh);
			
				// save the highscore to SRAM.
				highscoreLow = EEPROM_read(0);
				highscoreHigh = EEPROM_read(1);
				highscore = ((uint16_t)highscoreHigh << 8) | highscoreLow;

				OLEDGameOver(highscore);

				if( gameTimer > highscore ){
					EEPROM_write(0, scoreLow);
					EEPROM_write(1, scoreHigh);
					OLEDNewHighscore();
				}
				printf("Highscore %d", highscore);

				OLEDAfterGame();
				
				while(1)
				{
					joystickDriver();
					if (joystick_data.joystickPosition == LEFT)
					{
						state = START;
						break;
					}
					else if (joystick_data.joystickPosition == RIGHT)
					{
						state = INIT;
						break;
					}
					//_delay_ms(50);
				}
				
				// show HIGHSCORE and CURRENT score to OLED. - And an appropriate message
				//state = START;
				break;
			default: break;//printf("I don't know what state I'm in");
		}
	}while(state != START);
}