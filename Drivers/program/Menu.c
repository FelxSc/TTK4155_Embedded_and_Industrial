#include "OLED.h"
#include "ADC.h"
#include "JOYSTICK.h"
#include "Menu.h"
#include <avr/io.h>
#include <avr/common.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>
//#include "FONTS.h"

joystick_data_t joystick_data;

void menu1( void )
{
	menu_t *menu = malloc(sizeof(struct menu));
	menu->Title = "Main Menu:";	// Title of the current menu
	menu->Items[0] = "1. function 1.1";// "2. function 2", "3. function 3";	// The items names
	menu->Items[1] = "2. function 1.2";
	menu->Items[2] = "3. function 1.3";
	menu->numberOfItems = 3;		// Number items to choose
	menu->Top, menu->Sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu->selectNext, menu->selectPrevious = false;

	OLEDHome();
	OLEDPrintf(menu->Title);
	
	
	
	selectMenu(menu);
	
	switch(menu->Sel)
	{
		case 0:
			MainMenu();	// Select the previous menu - MainMenu is the upermost/first menu
			break;
		case 1:
			menu1();
			break;
		case 2:
			menu2();
			break;
		case 3:
			menu3();
			break;
		default:
			break;
			// Handle crysis :-(
	}
}

void menu2( void )
{
	menu_t *menu = malloc(sizeof(struct menu));
	menu->Title = "Main Menu:";	// Title of the current menu
	menu->Items[0] = "1. function 2.1";// "2. function 2", "3. function 3";	// The items names
	menu->Items[1] = "2. function 2.2";
	menu->Items[2] = "3. function 2.3";
	menu->numberOfItems = 3;		// Number items to choose
	menu->Top, menu->Sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu->selectNext, menu->selectPrevious = false;

	OLEDHome();
	OLEDPrintf(menu->Title);
	
	
	
	selectMenu(menu);
	
	switch(menu->Sel)
	{
		case 0:
			MainMenu();	// Select the previous menu - MainMenu is the upermost/first menu
			break;
		case 1:
			menu1();
			break;
		case 2:
			menu2();
			break;
		case 3:
			menu3();
			break;
		default:
			break;
			// Handle crysis :-(
	}
}

void menu3( void )
{
	menu_t *menu = malloc(sizeof(struct menu));
	menu->Title = "Main Menu:";	// Title of the current menu
	menu->Items[0] = "1. function 3.1";// "2. function 2", "3. function 3";	// The items names
	menu->Items[1] = "2. function 3.2";
	menu->Items[2] = "3. function 3.3";
	menu->numberOfItems = 3;		// Number items to choose
	menu->Top, menu->Sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu->selectNext, menu->selectPrevious = false;

	OLEDHome();
	OLEDPrintf(menu->Title);
	
	
	
	selectMenu(menu);
	
	switch(menu->Sel)
	{
		case 0:
			MainMenu();	// Select the previous menu - MainMenu is the upermost/first menu
			break;
		case 1:
			menu1();
			break;
		case 2:
			menu2();
			break;
		case 3:
			menu3();
			break;
		default:
			break;
			// Handle crysis :-(
	}
}


void drawMenu(menu_t* menu)	// pointer to the selected function
{
	OLEDHome();
	OLEDPrintf(menu->Title);
	for (int i = 0; i < 3; i++)
	{
		OLEDGotoPosition(i+1, 10);
		OLEDPrintf(menu->Items[i]);
	}
}

void selectMenu(menu_t* menu)
{
	menu->selectNext = false;
	menu->selectPrevious = false;
	OLEDClearAll();
	
	drawMenu(menu);

	while(!menu->selectNext & !menu->selectPrevious){
		joystickDriver();
		_delay_ms(500);
		printf("%d\n\r",joystick_data.joystickPosition);
		switch(joystick_data.joystickPosition)
		{
			case UP:
				OLEDClearLine(menu->Sel);
				OLEDGotoPosition(menu->Sel,10);
				OLEDPrintf(menu->Items[menu->Sel]);
				if(menu->Sel <= 0)
				menu->Sel = menu->numberOfItems;
				else if(menu->Sel >= menu->numberOfItems)
				menu->Sel = menu->Top;
				menu->Sel--;
				if(menu->Sel <= 0)
				menu->Sel = menu->numberOfItems;
				else if(menu->Sel >= menu->numberOfItems)
				menu->Sel = menu->Top;
				OLEDGotoPosition(menu->Sel,10);
				OLEDprintArrowRight();
				OLEDPrintf(menu->Items[menu->Sel]);
				OLEDprintArrowLeft();
				break;
			case CENTER:
//				OLEDPrintf("Hei");
				break;
			case DOWN:
				OLEDClearLine(menu->Sel);
				OLEDGotoPosition(menu->Sel,10);
				OLEDPrintf(menu->Items[menu->Sel]);
				if(menu->Sel <= 0)
				menu->Sel = menu->numberOfItems;
				else if(menu->Sel >= menu->numberOfItems)
				menu->Sel = menu->Top;
				menu->Sel++;
				if(menu->Sel <= 0)
				menu->Sel = menu->numberOfItems;
				else if(menu->Sel >= menu->numberOfItems)
				menu->Sel = menu->Top;
				OLEDGotoPosition(menu->Sel,10);
				OLEDGotoPosition(menu->Sel,10);
				OLEDprintArrowRight();
				OLEDPrintf(menu->Items[menu->Sel]);
				OLEDprintArrowLeft();
				break;
			case RIGHT:
				menu->selectNext = true; // Select chosen function in pSel
			case LEFT:
				menu->selectPrevious = true; // Select the previous function
			default:
				break;
				// Something else
		}

		if(menu->Sel <= 0)
			menu->Sel = menu->numberOfItems;
		else if(menu->Sel > menu->numberOfItems)
			menu->Sel = menu->Top;
		
	}
}



void MainMenu()
{
	menu_t *menu = malloc(sizeof(struct menu));
	menu->Title = "Main Menu:";	// Title of the current menu
	menu->Items[0] = "1. function 1";// "2. function 2", "3. function 3";	// The items names
	menu->Items[1] = "2. function 2";
	menu->Items[2] = "3. function 3";
	menu->numberOfItems = 3;		// Number items to choose
	menu->Top, menu->Sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu->selectNext, menu->selectPrevious = false;

	OLEDHome();
	OLEDPrintf(menu->Title);
	
	
	
	selectMenu(menu);
	
	switch(menu->Sel)
	{
		case 0:
			MainMenu();	// Select the previous menu - MainMenu is the upermost/first menu
			break;
		case 1:
			menu1();
			break;
		case 2:
			menu2();
			break;
		case 3:
			menu3();
			break;
		default:
		 break;
			// Handle crysis :-(
	}
}
/*
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
}*/
/*
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
}*/