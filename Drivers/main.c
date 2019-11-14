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

//extern void USART_Init(unsigned int ubrr);


//#include "OLED.h"
//#include "ADC.h"
//#include "Menu.h"
#include "USART.h"
//#include "JOYSTICK.h"
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

//CAN INTERRUPT
void InterruptInit( void )
{
	DDRD &= ~(1<<DDD3);
	
	cli();
	
	// Interrupt on falling edge
	EICRA &= ~(1<<ISC30);
	EICRA |= (1<<ISC31);

	
	// Enable Interrupt on PD3
	EIMSK |= (1<<INT3);
	
	// Enable global interrupts
	sei();
}

void Timer2Init()
{

	// Timer2
	TCCR2B = 0x07;
	TIMSK2 = 0x01;
	
}

CAN_message_t handleCANInterrupt()
{
	CAN_interrupt = 0;
	//printf("\n\nr\rCAN Interrupt");
	
	CAN_message_t receivedCAN1;
	
	uint8_t status;
	
	// Interrupt status
	status = MCP2515_Read(MCP_CANINTF);
	//printf("\n\rCANINTF : %x\n\n\r", status);
	
	//_delay_us(40);
	receiveCANmesssage(&receivedCAN1, 0x60);
	//_delay_us(150);
				
	return receivedCAN1;
}

void solenoidInit( void )
{
	DDRB |= (1 << PB6);
	
	PORTB = (1 << PB6);
}

void solenoidShoot( uint8_t leftbutton )
{
	if(leftbutton == 1)
		PORTB &= ~(1 << PB6);
	else
		PORTB |= (1<< PB6);
}

 void returnCANmsg( CAN_message_t data )
 {
 		 
	 	printf("\n\n\r************SENDING MSG: data *************\n\r");

	 	// CAN struct test
	 	/*printf("ID: %d\n\r",data.ID);
	 	printf("msg: %s\n\r", data.msg);
	 	printf("msgLen: %d\n\r",data.length);
		 */
	 	sendCANmessage(&data);
 }

ISR(INT3_vect)
{
	CAN_interrupt = 1;

}

ISR(TIMER1_OVF_vect)
{
	
}

ISR(TIMER2_OVF_vect)
{
		pidTimer = 1;
}

uint8_t IR(void)
{
	uint16_t ir = 0;
	for(int i = 0; i<2;i++)
		ir = ir + readADC();
		
	ir = ir/3;
	
	if(ir <= 250 )
	{
		return 1;
	}
	return 0;
	
	
	
}

int main()
{
	
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	
	
	InterruptInit();
	Timer2Init();
	
	//ExernalMemoryInit();
	//adcInit();
	//OLEDInit();
	//menuInit();
	SPI_MasterInit();
	ADCinit();
	solenoidInit();
	cli();
	TWI_Master_Initialise();
	sei();
	pidInit();

	motorDriverInit();
	motorCalibrate();

	
	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);
		
	//joystickCalibrate();
	

	
	
	uint8_t status, dataReceive, dutyCycle;	
	uint16_t encoderData;
	CAN_message_t receivedCAN, joystick_data, slider_data, game_data;
	
	motor.targetPosition = 60;
	game_data.msg[0] = 120;
	
	ServoTimer1Init();
	
	
	while(1)
	{
	
		

		if(CAN_interrupt == 1){
			

			
			game_data = handleCANInterrupt();	
			
			//joystick_data = receivedCAN;// break; }
			//game_data = receivedCAN;
			//printf("Left slider = %d\n\r", slider_data.msg[0]);
			//printf("Right slider = %d\n\r", slider_data.msg[1]);
			
			//motor.targetPosition = receivedCAN.msg[1];
			//motor.direction = receivedCAN.msg[2];
			
			//setMotorDirection(false, NULL);
			//setMotorSpeed(false, NULL);
			
			
			//dutyCycle = calculateDutyCycleCounter(joystick_data.msg[0]);
			dutyCycle = calculateDutyCycleCounter(game_data.msg[0]);
			setDutyCycle(dutyCycle);
			//printf("Servo-duty cycle = %d\n\r", dutyCycle);
			//motor.targetPosition = joystick_data.msg[1];
			motor.targetPosition = 255 - game_data.msg[1];
			
			//printf("Button = %d\n\r", game_data.msg[2]);
			solenoidShoot(game_data.msg[2]);
			
		}
		
			
		if(pidTimer == 1 & game_data.msg[3] == 1)
		{
			
			pidTimer = 0;
			pid_controller();
			setMotorDirection(false, NULL);
			setMotorSpeed(false, NULL);
				
				
			//pid.currentPosition = encoderRead();
			//pid.currentPosition = pid.currentPosition/36;
			//printf("\n\rCurrentPosition: %d\n\r",pid.currentPosition);

			//printf("target %d\n\r",motor.targetPosition);
		}
			
			if(IR() == 1)
				{
					cli();
					game_data.ID = 3;
					game_data.msg[4] = GAMEOVER;
					sendCANmessage(&game_data);
					printf("HELLO IR?");
					sei();
				}
				
		
		_delay_ms(15);
	}

	return 0;
}