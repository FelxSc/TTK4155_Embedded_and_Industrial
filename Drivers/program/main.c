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


volatile int CAN_interrupt = 0;

void InterruptInit( void )
{
	DDRD &= ~(1<<PD2);
	
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

ISR(INT0_vect)
{
	CAN_interrupt = 1;

}

int main()
{
	
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	InterruptInit();
	ExernalMemoryInit();
	adcInit();
	OLEDInit();
	menuInit();
	SPI_MasterInit();
	
	
	MCP2515init(MODE_LOOPBACK);	
	//joystickCalibrate();
	

	
	
	uint8_t status, dataReceive, dataSend = 1;
	CAN_message_t data,data1, receivedCAN;
	
	data.ID = 2;
	data.msg[0] = 'H';
	data.msg[1] = 'e';
	data.msg[2] = 'i';
	data.length = strlen(data.msg);
	
	data1.ID = 1;
	data1.msg[0] = 'V';
	data1.msg[1] = 'e';
	data1.msg[2] = 'r';
	data1.msg[3] = 'd';
	data1.msg[4] = 'e';
	data1.msg[5] = 'n';
	data1.length = strlen(data.msg);

	
	while(1)
	{
		
		//int selectedMenu = selectMenu();
		//int num = (int) selectedMenu;
		
		//printf("-%d-", selectedMenu);
		
		//printf("Running!\n\r");
/*		printf("x-offset = %d\n\r", joystickCalibration.x_offset);
		printf("y-offset = %d\n\r", joystickCalibration.y_offset);
		joystickDriver();
		
		printf("-------------------------\n\r");
		*/
		
		
		sendCANmessage(&data);
		//sendCANmessage(&data1);
		
		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("TXB0CTRL: %x",status);
		
		receiveCANmesssage(&receivedCAN);
		
		MCP2515_bitMask(MCP_TXB0CTRL,0x08, 0);
		
		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("TXB0CTRL: %x",status);
		
		printf("\n\n\rSENDING MSG\n\r");

		// CAN struct test
		printf("ID: %d\n\r",data.ID);
		printf("msg: %s\n\r", data.msg);
		printf("msgLen: %d\n\r",data.length);
/*
		// Read RX status register
		status = MCP2515_readRXstatus();
		printf("RX status : %x\n\r",status);
		
		status = MCP2515_readStatus(MCP_CANSTAT);
		printf("CANSTAT : %x\n\r",status);
		
		// Status of buffers
		status = MCP2515_readRxTxStatus();
		printf("MCP2515 status: %x\n\n\r", status);
		*/
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
		
		
		
		
		
		
		
		
		printf("\n\n\rRECEIVING MSG\n\r");
		
		printf("ID: %d\n\r",receivedCAN.ID);
		printf("msg: %s\n\r", receivedCAN.msg);
		printf("msgLen: %d\n\r",receivedCAN.length);
		
		//CAN_Write(0x36, &data );
		//CAN_Write(MCP_LOAD_TX0, &data1 );
		//dataReceive = CAN_Read();
		//printf("DataReceive : %c\n\n\r", dataReceive);
		/*dataReceive = MCP2515_Read(MCP_READ_RX0);
		printf("DataReceive : %c\n\n\r", dataReceive);
		dataReceive = MCP2515_Read(MCP_READ_RX1);
		printf("DataReceive : %c\n\n\r", dataReceive);

		*/
		
		if(CAN_interrupt == 1)
		{
			CAN_interrupt = 0;
			printf("CAN Interrupt");
			
			// Interrupt status
			status = MCP2515_Read(MCP_CANINTF);
			printf("CANINTF : %x\n\n\r", status);
			
			dataReceive = MCP2515_Read(MCP_READ_RX0);
			printf("DataReceive : %c\n\n\r", dataReceive);
			
			_delay_ms(1000);
		}
		
		_delay_ms(200);
	}
	
	return 0;
}