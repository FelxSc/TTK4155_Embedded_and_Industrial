#include "OLED.h"
#include "ADC.h"
//#include "FONTS.h"

joystick_data_t joystick_data;

void menu1( void )
{
	menu_t menu;
	menu.Title = "Menu 1:";	// Title of the current menu
	menu.Items[] = "1. function 1.1", "2. function 1.2", "3. function 1.3";	// The items names
	menu.numberOfItems = 3;		// Number items to choose
	menu.top, menu.sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu.selectNext, menu.SelectPrevious = false;

	selectMenu(&menu);
	
	switch(menu.sel)
	{
		case 0:
			MainMenu()	// Select the previous menu - MainMenu is the upermost/first menu
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
			// Handle crysis :-(

}

void menu2( void )
{
	menu_t menu;
	menu.Title = "Menu2:";	// Title of the current menu
	menu.Items[] = "1. function 2.1", "2. function 2.2", "3. function 2.3";	// The items names
	menu.numberOfItems = 3;		// Number items to choose
	menu.top, menu.sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu.selectNext, menu.SelectPrevious = false;

	selectMenu(&menu);
	
	switch(menu.sel)
	{
		case 0:
			MainMenu()	// Select the previous menu - MainMenu is the upermost/first menu
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
			// Handle crysis :-(

}

void menu3( void )
{
	menu_t menu;
	menu.Title = "Main Menu:";	// Title of the current menu
	menu.Items[] = "1. function 3.1", "2. function 3.2", "3. function 3.3";	// The items names
	menu.numberOfItems = 3;		// Number items to choose
	menu.top, menu.sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu.selectNext, menu.SelectPrevious = false;

	selectMenu(&menu);
	
	switch(menu.sel)
	{
		case 0:
			MainMenu()	// Select the previous menu - MainMenu is the upermost/first menu
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
			// Handle crysis :-(
}


void drawmenu(	const char* pTitle, // Pointer to menuTitle
				const char* psItems[], // Pointer to item strings
				uint8_t numberOfItems, // Number of functions to choose
				uint8_t* pTop,	// Pointer to the top og the list
				uint8_t* pSel,	// pointer to the selected function
{
	OLEDHome();
	OLEDPrintf(pTitle);
	for (int i = 0; i < numberOfItems; i++)
	{
		OLEDGotoPosition(i+1, 10);
		OLEDPrintf(psItems);
	}
}

void selectMenu(menu_t* menu)				)
{
	menu.selectNext = false;
	menu.selectPrevious = false;
	OLEDClearAll();
	drawMenu();

	while(!menu.selectNext & !menu.selectPrevious){
		
		OLEDPrintf()
		switch(joystick_data.joystickPosition)
		{
			case UP:
				OLEDClearLine(menu.Sel);
				OLEDGotoLine(menu.Sel);
				OLEDPrintf(menu.Items[menu.Sel]);
				menu.Sel--;
				OLEDGotoLine(menu.Sel);
				OLEDPrintf(menu.Items[menu.Sel]);
				break;
			case CENTER:
				break;
			case DOWN:
				OLEDClearLine(menu.Sel);
				OLEDGotoLine(menu.Sel);
				OLEDPrintf(menu.Items[menu.Sel]);
				menu.Sel++;
				OLEDGotoLine(menu.Sel);
				OLEDPrintf(menu.Items[menu.Sel]);
				break;
			case RIGHT:
				menu.selectNext = true; // Select chosen function in pSel
			case LEFT:
				menu.selectPrevious = true; // Select the previous function
			default:
				// Something else
		}

		if(menu.Sel <= 0)
			menu.sel = menu.numberOfItems;
		elseif(menu.Sel > menu.numberOfItems)
			menu.Sel = menu.Top;
		
	}
}



void MainMenu()
{
	menu_t menu;
	menu.Title = "Main Menu:";	// Title of the current menu
	menu.Items[] = "1. function 1", "2. function 2", "3. function 3";	// The items names
	menu.numberOfItems = 3;		// Number items to choose
	menu.top, menu.sel = 1; // top: top og the menu list: 1 (previous menu: 0). Sel: The selected menu - items 1 to nItems
	menu.selectNext, menu.SelectPrevious = false;

	selectMenu(&menu);
	
	switch(menu.sel)
	{
		case 0:
			MainMenu()	// Select the previous menu - MainMenu is the upermost/first menu
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