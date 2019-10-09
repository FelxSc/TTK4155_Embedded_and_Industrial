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
	
	printf("Initialization of MCP2515...\n\r");
	MCP2515init(MODE_LOOPBACK);	
	//joystickCalibrate();
	

	
	
	uint8_t status, dataReceive, dataSend = 1;
	CAN_message_t data,data1, receivedCAN, receivedCAN1;
	
	data.ID = 10;
	data.msg[0] = 'H';
	data.msg[1] = 'a';
	data.msg[2] = 'd';
	data.length = strlen(data.msg);
	
	data1.ID = 20;
	data1.msg[0] = '1';
	data1.msg[1] = '2';
	data1.msg[2] = '3';
	data1.msg[3] = '5';
	data1.msg[4] = '5';
	data1.msg[5] = '6';
	data1.msg[6] = '7';	
	//data1.msg[7] = '8';
	data1.length = strlen(data1.msg);

	
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
		
		
		
		
		
		
/*	
		// Test - Send MSG data
		
		printf("\n\n\rSENDING MSG: data\n\r");

		// CAN struct test
		printf("ID: %d\n\r",data.ID);
		printf("msg: %s\n\r", data.msg);
		printf("msgLen: %d\n\r",data.length);
		
		
		// Send MSG data
		sendCANmessage(&data);
		
		// Read status register for TX0
		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("TXB0CTRL: %x",status);
		
		
		
		
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
		
		// clear interrupt register
		MCP2515_bitMask(MCP_CANINTF, 0xff, 0x00);
		
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
		
		
		
		
		
		_delay_ms(1000);
		
		// Test receive MSG receivedCAN

		printf("\n\n\rRECEIVING MSG -receivedCAN-\n\r");
		receiveCANmesssage(&receivedCAN);




		printf("ID: %d\n\r",receivedCAN.ID);
		printf("msg: %s\n\r", receivedCAN.msg);
		printf("msgLen: %d\n\r",receivedCAN.length);
		
		// Read status reg
		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("TXB0CTRL: %x",status);
		
		
		
		
		
				
				
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
				
		// clear interrupt register
		MCP2515_bitMask(MCP_CANINTF, 0xff, 0x00);
				
				
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
		
		
*/		
		
		
		// Test send MSG data1

		printf("\n\n\r************SENDING MSG: data1 *************\n\r");

		// CAN struct test
		printf("ID: %d\n\r",data1.ID);
		printf("msg: %s\n\r", data1.msg);
		printf("msgLen: %d\n\r",data1.length);
		
		// Send MSG data1
		sendCANmessage(&data1);
				printf("IDhigh Just sent: %x\n\r", MCP2515_Read(MCP_TXB0SIDH));
				printf("IDlow just sent: %x\n\r", MCP2515_Read(MCP_TXB0SIDL));

		// Read status
		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("TXB0CTRL: %x\n\r",status);



		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\r", status);
		
		// clear interrupt register
		MCP2515_bitMask(MCP_CANINTF, 0xff, 0x00);
		
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF cleared : %x\n\r", status);
		

		_delay_ms(1000);


		// Test receivedCAN1
		printf("\n\n\r********RECEIVING MSG -receivedCAN1- ***********\n\r");

		receiveCANmesssage(&receivedCAN1);
	


		printf("ID: %d\n\r",receivedCAN1.ID);
		printf("msg: %s\n\r", receivedCAN1.msg);
		printf("msgLen: %d\n\r",receivedCAN1.length);



		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\r", status);
		
		// clear interrupt register
		MCP2515_bitMask(MCP_CANINTF, 0xff, 0x00);
		
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF cleared: %x\n\r", status);






		status = MCP2515_readStatus(MCP_TXB0CTRL);
		printf("\n\n\rTXB0CTRL: %x\n\r",status);

		// Read RX status register
		status = MCP2515_readRXstatus();
		printf("RX status : %x\n\r",status);
		
		status = MCP2515_readStatus(MCP_CANSTAT);
		printf("CANSTAT : %x\n\r",status);
		
		// Status of buffers
		status = MCP2515_readRxTxStatus();
		printf("MCP2515 status: %x\n\r", status);
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\r", status);
		

		_delay_ms(1000);
		
		
		
		
		
		
		//CAN_Write(0x36, &data );
		//CAN_Write(MCP_LOAD_TX0, &data1 );
		//dataReceive = CAN_Read();
		//printf("DataReceive : %c\n\n\r", dataReceive);
		/*dataReceive = MCP2515_Read(MCP_READ_RX0);
		printf("DataReceive : %c\n\n\r", dataReceive);
		dataReceive = MCP2515_Read(MCP_READ_RX1);
		printf("DataReceive : %c\n\n\r", dataReceive);

		*/
		
/*		if(CAN_interrupt == 1)
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
		*/	
		_delay_ms(200);
	}

	return 0;
}