/*
 * game.c
 *
 * Created: 13.11.2019 11:49:08
 *  Author: Odd Inge Halsos & Felix Schöpe
 */
#ifndef F_CPU
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
#include "HIGHSCORE.h"
#include "Music.h"


void game(void){
		
	do{
		switch(state){
			case START:
				gameStatus = GAMESTOP;
				state = INIT;
				break;
			case INIT:
				gameStatus = GAMESTART;
				gameTimer = 0;
				OLEDStartGame();
				OLEDClearAll();
				state = RUNGAME;
				enableSendCANTimer1();
				enableGameTimer3();
				break;
		
			case RUNGAME:				
				while(gameStatus != GAMEOVER)
				{				
					// Test Left Touch-button to activate solenoid
					if(test_bit(PINB,PINB3))
					{	slider_data.leftbutton = 1;	}
					else
					{	slider_data.leftbutton = 0; }
			
					if(CAN_interrupt == 1){
						
						game_data = handleCANreceiveInterrupt();
						gameStatus = game_data.msg[3];
					}
			
					if(Timer1_interrupt)
						{ SendCAN(); OLEDScoreCounter();}
											
					_delay_ms(20);
								
				}
				state = GAMEOVER;
			
				break;
			case GAMEOVER:
				disableSendCANTimer1();
				disableGameTimer3();

				OLEDGameOver(ReadHighscore(0));
				
				if (saveHighscore(gameTimer) == 1)
				{
					OLEDNewHighscore();
					PlaySong(ZELDA_ID);
					
				}
				else
					loserSound();
				
				OLEDAfterGame();
				
				while(1)
				{		// Return to menu or start new game
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
				}
				
				break;
			default: break;
		}
	}while(state != START);
}
#endif