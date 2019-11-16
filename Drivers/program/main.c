//#ifndef F_CPU
#define F_CPU 4915200UL // 4.9152 Mhz
//#endif

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
#include <avr/eeprom.h>

//extern void USART_Init(unsigned int ubrr);


#include "OLED.h"
#include "ADC.h"
#include "Menu.h"
#include "USART.h"
#include "JOYSTICK.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "bitMacro.h"
#include "SRAM.h"
#include "game.h"
#include "Interrupt.h"
#include "EEPROM.h"




uint8_t timer1Flag;

void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
	
}


ISR(INT0_vect)
{
	CAN_interrupt = 1;
}

ISR(INT1_vect)
{
	joystickButtonInterrupt = 1;
}

ISR(TIMER1_OVF_vect)
{
	timer1Flag++;
	if(timer1Flag > 1)
	{ timer1Flag = 0;	Timer1_interrupt = 1; }
}

ISR(TIMER3_COMPA_vect)
{
	gameTimer++;
}


void main()
{
	
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	
	cli();
	InterruptInit();
	Timer1Init();
	Timer3Init();
	sei();
	ExernalMemoryInit();
	adcInit();
	OLEDInit();
	menuInit();
	SPI_MasterInit();
	solenoidButtonInit();
	gameButtonInit();
	joystickButtonInit();

	
	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);	
	


	
	
	uint8_t status, dataReceive, dataSend = 1;
	uint8_t current_Line;
	
	// game state initialized as IDLE
	state = START;
	
	char dataWrite = 'a';
	char dataRead;
	uint16_t adr = 0x0;
	
	uint16_t highscore, address;
	uint8_t highscoreLow, highscoreHigh, scoreLow, scoreHigh;
	while(1)
	{
		//printf("MAINloop - state = %d\n\r",state);
		
		/*printf("dataSend: %x\n\r", dataWrite);
		SRAM_write(adr, dataWrite);
		//dataWrite++;
		_delay_ms(20);
		dataRead = SRAM_read(adr);
		printf("Address: %x - dataReceive: %x\n\r",adr, dataRead);
		dataRead = 0;
		adr++;*/
		
		//SRAM_test();
		
		
		/*printf("GAMEOVER");
		printf("GameTimer: %d\n\r", gameTimer);
		
		scoreLow = gameTimer;
		scoreHigh = gameTimer >> 8;
		
		printf("ScoreLow = %x\n\r", scoreLow);
		printf("ScoreHigh = %x\n\r", scoreHigh);
		
			SRAM_write(address, scoreLow);
			SRAM_write(address+1, scoreHigh);

		// save the highscore to SRAM.
		highscoreHigh = SRAM_read(address+1);
		highscoreLow = SRAM_read(address);
		
		highscore = ((uint16_t)highscoreHigh << 8) | highscoreLow;
		
		
		//if( gameTimer > highscore ){

		//}
		printf("highscoreLow: %x\n\r", highscoreLow);
		printf("highscoreHigh: %x\n\r", highscoreHigh);
		printf("highscore: %d\n\r", highscore);
		
		*/
		
		//game();		
		
		current_Line = selectMenu();
			//OLEDClearAll();
		_delay_ms(20);
		//_delay_ms(500);
		
	}
}