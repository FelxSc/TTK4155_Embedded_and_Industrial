#include "OLED.h"
#include "ADC.h"
//#include "FONTS.h"

joystick_data_t joystick_data;

void menu1( void )
{
	OLEDClearAll();
	OLEDHome();
	OLEDPrintf("MenuOption1");
}

void menu2( void )
{
	OLEDClearAll();
	OLEDHome();
	OLEDPrintf("MenuOption2");
}

void menu3( void )
{
	OLEDClearAll();
	OLEDHome();
	OLEDPrintf("MenuOption3");
}

void menu4( void )
{
	OLEDClearAll();
	OLEDHome();
	OLEDPrintf("MenuOption4");
	
}

void printMenu0(void)
{
	OLEDClearAll();
	OLEDHome();
	OLEDinvPrintf("* MAIN MENU *");
	OLEDGotoPosition(1,0);
	OLEDPrintf("Menu1");
	OLEDGotoPosition(2,0);
	OLEDPrintf("Menu2");
	OLEDGotoPosition(3,0);
	OLEDPrintf("Menu3");
	OLEDGotoPosition(4,0);
	OLEDPrintf("Menu4");
}

void menu( void )
{
	printMenu0();
	
	switch (joystick_data.menuCounter){
		case 1: menu1();
			break;
		case 2: menu2();
			break;
		case 3: menu3();
			break;
		case 4: menu4();
			break;
		default:
			break;
	}
}