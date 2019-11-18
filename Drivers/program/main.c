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
#include <avr/eeprom.h>

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
#include "Music.h"
#include "solenoid.h"


volatile uint8_t timer1Flag;

void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
}

// Receive CAN message Interrupt
ISR(INT0_vect)
{
	CAN_interrupt = 1;
}

// Joystick pushbutton ISR
ISR(INT1_vect)
{
	joystickButtonInterrupt = 1;
}

//SendCANmessage ISR
ISR(TIMER1_OVF_vect)
{
	timer1Flag++;
	if(timer1Flag > 1)
	{ timer1Flag = 0;	Timer1_interrupt = 1; }
}

// Game score ISR
ISR(TIMER3_COMPA_vect)
{
	gameTimer++;
}

// Buzzer ISR
ISR(TIMER0_COMP_vect)
{
 
}


int main()
{
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	
	receiveCanInterruptInit();
	SendCANTimer1Init();
	gameTimer3Init();

	ExernalMemoryInit();
	adcInit();
	OLEDInit();
	menuInit();
	SPI_MasterInit();
	solenoidButtonInit();
	gameButtonInit();
	buzzerInit();

	buzzerTimer0Init();
	joystickButtonInit();
	
	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);	
	

	disableBuzzer();
	disableSendCANTimer1();
	disableGameTimer3();

	// game state initialized as START
	state = START;

	//EEPROM_reset(20);
	
	// Main Loop
	while(1)
	{
		selectMenu();

		_delay_ms(300);
		_delay_ms(20);
		
	}
	return 0;
}

#endif