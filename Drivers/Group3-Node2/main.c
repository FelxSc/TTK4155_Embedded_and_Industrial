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


volatile int CAN_interrupt = 0;

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


CAN_message_t handleCANInterrupt()
{
	CAN_interrupt = 0;
	//printf("\n\nr\rCAN Interrupt");
	
	CAN_message_t receivedCAN1;
	
	uint8_t status;
	
	// Interrupt status
	status = MCP2515_Read(MCP_CANINTF);
	//printf("\n\rCANINTF : %x\n\n\r", status);
	
	
	receiveCANmesssage(&receivedCAN1, 0x60);
	
	return receivedCAN1;
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

void IR(void)
{
	uint16_t ir = 0;
	uint8_t score;
	for(int i = 0; i<2;i++)
		ir = ir + readADC();
		
	ir = ir/3;
	
	if(ir <= 250 )
	{
		score++;
	}
	
	
	
}

int main()
{
	
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	InterruptInit();
	//ExernalMemoryInit();
	//adcInit();
	//OLEDInit();
	//menuInit();
	SPI_MasterInit();
	ServoTimer1Init();
	ADCinit();

	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_NORMAL);	
	//joystickCalibrate();
	

	
	
	uint8_t status, dataReceive, dutyCycle;	
	CAN_message_t receivedCAN, joystick_data;
	
	
	while(1)
	{
		uint16_t data = readADC();
				
		/*if(CAN_interrupt == 1){
			receivedCAN = handleCANInterrupt();

			joystick_data = receivedCAN;// break; }
			
			dutyCycle = calculateDutyCycleCounter(joystick_data.msg[0]);
			setDutyCycle(dutyCycle);
		}	*/
		_delay_ms(100);
	}

	return 0;
}