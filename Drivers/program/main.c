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
	
	//joystickCalibrate();
	
	menuInit();
	
	
	
	while(1)
	{
		
		int selectedMenu = selectMenu();
		//int num = (int) selectedMenu;
		
		//printf("-%d-", selectedMenu);
		
		//printf("Running!\n\r");
/*		printf("x-offset = %d\n\r", joystickCalibration.x_offset);
		printf("y-offset = %d\n\r", joystickCalibration.y_offset);
		joystickDriver();
		
		printf("-------------------------\n\r");
		*/
		_delay_ms(200);
		

	}
	
	return 0;
}