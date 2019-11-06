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


volatile int CAN_interrupt = 0;
volatile int Timer1_interrupt = 0;



void solenoidButtonInit(void)
{	
	// PB3 as input
	DDRB &= ~(1 << PB3);

}



void Timer1Init( void )
{
	// Set normal mode
	TCCR1A = 0x00;
	
	// Control register: Prescale = 64
	TCCR1B = 0x02;
	
	// Enable TIMER1 Interrupt
	TIMSK = 0x80;
	
	// Initiate TOV1 flag as cleared
	TIFR = 0x00;	
	
}








void InterruptInit( void )
{
	DDRD &= ~(1<<PD2); // MCP2515 interrupt
	
	cli();
	
	// Interrupt on falling edge
	MCUCR &= ~(1<<ISC00);
	MCUCR |= (1<<ISC01);

	
	// Enable Interrupt on PD2
	GICR |= (1<<INT0);
	
	// Enable global interrupts
	sei();
}

void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
	
}



CAN_message_t handleCANInterrupt()
{
	CAN_interrupt = 0;
	printf("\n\nr\rCAN Interrupt");
	
	CAN_message_t receivedCAN1;
	
	//CAN_message_t * receivedCAN1 = malloc(sizeof(CAN_message_t));
	//CAN_message_t * receivedCAN2 = malloc(sizeof(CAN_message_t));
	
	//receivedCAN1 = received1;
	//receivedCAN2 = received2;
	uint8_t status;
	
	// Interrupt status
	status = MCP2515_Read(MCP_CANINTF);
	printf("\n\rCANINTF : %x\n\n\r", status);
	
	if(status == 0xa0)
		printf("Transaction Error\n\r");
	else
		receiveCANmesssage(&receivedCAN1, 0x60);
	
	return receivedCAN1;
}


 void handleTimer1Interrupt( void )
 {
	 Timer1_interrupt = 0;
 		CAN_message_t data;
		 
		 //joystickDriver();
		 sliderDriver();
		 
		 data.ID = 1;
		 //data.msg[0] = joystick_data.x_position;
		 //data.msg[1] = joystick_data.y_position;
		 //data.msg[2] = joystick_data.joystickPosition;
		 data.msg[0] = slider_data.leftslider; // Servo data -> dutycycle
		 data.msg[1] = slider_data.rightslider; // motor position data
		 data.msg[2] = slider_data.leftbutton;
		 data.msg[3] = '0';
		 data.msg[4] = '0';
		 data.msg[5] = '0';
		 data.msg[6] = '0';
		 data.msg[7] = '\0';
		 data.length = 8;
		 
		  
	 	printf("\n\n\r************SENDING MSG: data *************\n\r");

	 	// CAN struct test
	 	printf("ID: %d\n\r",data.ID);
	 	printf("X position: %d\n\r", data.msg[0]);
	 	printf("Y position: %d\n\r", data.msg[1]);
	 	printf("Direction: %d\n\r", data.msg[2]);
	 	printf("msgLen: %d\n\r",data.length);
	 	sendCANmessage(&data);
		 
		printf("TX status: %x",MCP2515_readRxTxStatus());
 }
	


ISR(INT0_vect)
{
	CAN_interrupt = 1;

}

ISR(TIMER1_OVF_vect)
{
	Timer1_interrupt = 1;
}


void main()
{
	
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	InterruptInit();
	Timer1Init();
	ExernalMemoryInit();
	adcInit();
	OLEDInit();
	menuInit();
	SPI_MasterInit();
	solenoidButtonInit();
	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);	
	//joystickCalibrate();
	

	
	
	uint8_t status, dataReceive, dataSend = 1;
	uint8_t current_Line;
	
	CAN_message_t data1, data2, data3, receiveCAN1;
	
			 /*CAN_message_t data;
			 
			 data.ID = 1;
			 data.msg[0] = '1';
			 data.msg[1] = '0';
			 data.msg[2] = '0';
			 data.msg[3] = '0';
			 data.msg[4] = '0';
			 data.msg[5] = '0';
			 data.msg[6] = '0';
			 data.msg[7] = '\0';
			 data.length = 8;
			
			
			handleTimer1Interrupt();*/
	while(1)
	{
		
		if(test_bit(PINB,PINB3))
			{ slider_data.leftbutton = 1; /*Timer1_interrupt = 1;*/}
			
		else
		{
			slider_data.leftbutton = 0;
		}
	
		if(CAN_interrupt == 1){
			receiveCAN1 = handleCANInterrupt();
				
			//printf("\n\n\rback from handleCANInterrupt\n\r");
			//printf("ID: %d\n\r",receiveCAN1.ID);
			//printf("msg: %s\n\r", receiveCAN1.msg);
			//printf("msgLen: %d\n\r",receiveCAN1.length);
			
			
		}
		
		if(Timer1_interrupt)
			handleTimer1Interrupt();		

		current_Line = selectMenu();
			
		_delay_ms(20);
	}
}