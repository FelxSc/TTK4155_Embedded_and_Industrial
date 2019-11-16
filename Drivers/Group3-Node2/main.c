//#ifndef F_CPU
#define F_CPU 16000000UL // 16 Mhz
//#endif

#include <avr/io.h>
#include <string.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/pgmspace.h>


#include "main.h"
#include "USART.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "SERVO.h"
#include "ADC.h"
#include "MOTOR.h"
#include "TWI_Master.h"
#include "pid.h"
#include "game.h"

volatile int CAN_interrupt = 0;
volatile int pidTimer = 0;
volatile int timercounter = 0;


// **************** Initialization Functions **************

//CAN INTERRUPT
void InterruptInit( void )
{
	DDRD &= ~(1<<DDD3);
	
	// Interrupt on falling edge
	EICRA &= ~(1<<ISC30);
	EICRA |= (1<<ISC31);

	
	// Enable Interrupt on PD3
	EIMSK |= (1<<INT3);
	
}

// Timer2 used to call PID controller periodically at gameplay
void Timer2Init()
{
	TCCR2B = 0x07;
	TIMSK2 = 0x01;
}


void solenoidInit( void )
{
	DDRB |= (1 << PB6);
	PORTB = (1 << PB6);
}

// ********************** 
CAN_message_t handleCANInterrupt()
{
	CAN_interrupt = 0;
	
	CAN_message_t receivedCAN;
	
	receiveCANmesssage(&receivedCAN);
				
	return receivedCAN;
}



void solenoidShoot( uint8_t leftbutton )
{
	if(leftbutton == 1)
		PORTB &= ~(1 << PB6);
	else
		PORTB |= (1<< PB6);
}

// IR handler
// Read ADC three times to find average value. If average is below a set threshold the function returns 1 to signal interrupted IR beam
uint8_t IR(void)
{
	uint16_t ir = 0;
	for(int i = 0; i<2;i++)
	ir = ir + readADC();
	ir = ir/3;
	
	if(ir <= 100 )
		return 1;
	
	return 0;
}

// Return received message to test connection between nodes
 void returnCANmsg( CAN_message_t data )
 {
	 	printf("\n\n\r************ RETURNING CAN MESSAGE *************\n\r");
	 	sendCANmessage(&data);
 }


// ******************** Interrupt Service Routines **************************
ISR(INT3_vect)
{
	CAN_interrupt = 1;

}

ISR(TIMER1_OVF_vect)
{
	// Servo interrupt routine - Empty on purpose
}

ISR(TIMER2_OVF_vect)
{
		pidTimer = 1;
}



int main()
{
	
// ----- Initialization -----
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	
	cli();
	InterruptInit();
	Timer2Init();
	TWI_Master_Initialise();
	sei();
	
	SPI_MasterInit();
	ADCinit();
	solenoidInit();
	pidInit();
	motorDriverInit();
	motorCalibrate();

	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);
		
	

	
	
	uint8_t dutyCycle;	
	CAN_message_t game_data;
	
	motor.targetPosition = 60;
	game_data.msg[0] = 120;
	
	ServoTimer1Init();
	
	
	// Main Loop	
	while(1)
	{

		if(CAN_interrupt == 1){

			game_data = handleCANInterrupt();	
			dutyCycle = calculateDutyCycleCounter(game_data.msg[0]);
			setDutyCycle(dutyCycle);
			motor.targetPosition = 255 - game_data.msg[1];
			
			solenoidShoot(game_data.msg[2]);
		}
		
			
		if(pidTimer == 1 & game_data.msg[3] == GAMESTART)
		{
			pidTimer = 0;
			pid_controller();
			setMotorDirection(false, NULL);
			setMotorSpeed(false, NULL);
							
			if(IR() == 1)
				{
					game_data.ID = 3;
					game_data.msg[3] = GAMEOVER;
					sendCANmessage(&game_data);
				}
		}
		
		_delay_ms(60);
	}	// Main Loop end

	return 0;
}