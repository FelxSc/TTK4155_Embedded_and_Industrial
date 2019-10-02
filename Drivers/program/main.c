//#ifndef F_CPU
#define F_CPU 4915200UL // 4.9152 Mhz
//#endif

#include <avr/io.h>
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


void ExernalMemoryInit( void )
{
	MCUCR |= (1<<SRE); // Enable External memory
	SFIOR |= (1<<XMM2); // mask PORTC 4 - 7 JTAG
	
}

int main()
{
// ----- Initialization ----- //
	USART_Init( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	ExernalMemoryInit();
	adcInit();
	OLEDInit();
	SPI_MasterInit();
	
	//joystickCalibrate();
	
	menuInit();
	
	
	void MCP2515_reset();

	//init loopback for testing purposes	
	MCP2515_Write(MCP_CANCTRL,MODE_LOOPBACK);
	
	uint8_t status, dataReceive, dataSend = 1;
	
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

		// Read RX status register
		status = MCP2515_readRXstatus();
		printf("RX status : %x\n\r",status);
		
		status = MCP2515_readStatus(MCP_CANSTAT);
		printf("CANSTAT : %x\n\r",status);
		
		// Status of buffers
		status = MCP2515_readRxTxStatus();
		printf("MCP2515 status: %x\n\n\r", status);
		
		// Read CAN interrupt status register
		status = MCP2515_Read(MCP_CANINTF);
		printf("CANINTF : %x\n\n\r", status);
		
		
		
		
		
		
		MCP2515_Write(MCP_LOAD_TX0, dataSend );
		dataReceive = MCP2515_Read(MCP_READ_RX1);
		printf("DataReceive : %d\n\n\r", dataReceive);
		dataSend++;

		_delay_ms(200);

	}
	
	return 0;
}