#include "OLED.h"
#include "ADC.h"
#include "JOYSTICK.h"
#include "Menu.h"
#include "Interrupt.h"
#include "game.h"

#include <avr/io.h>
#include <avr/common.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>
//#include "FONTS.h"

joystick_data_t joystick_data;

volatile menu_t* currentMenu;
volatile int currentLine = 1;


menu_t* createMenu(char* title, menu_t* parent, selectedMenu_t* ID)
{
	menu_t* menu = malloc(sizeof(menu_t));
	menu->title = title;
	menu->parent = parent;
	menu->ID = ID;
	menu->firstChild = NULL;
	menu->rightSibling = NULL;
	return menu;
}

// Assign the first menu option in a list of options
void setFirstChild(menu_t *parent, menu_t* firstChild)	// Make sure this works - Otherwise "parent" should be menu 
{
	parent->firstChild = firstChild;
}

// Assign the next menu option in the list
void setRightSibling(menu_t* thisChild, menu_t* rightSibling)	// Make sure this works - Otherwise "thisChild" should be menu 
{
	thisChild->rightSibling = rightSibling;
}

void menuInit( void )
{
	// Create a list of all the menu options in Main Menu
	menu_t* mainMenu = createMenu("Main Menu", NULL, MAIN_MENU);
	menu_t* playgame = createMenu("Play Game", mainMenu, PLAYGAME);
	menu_t* options = createMenu("Options", mainMenu, OPTIONS);
	menu_t* highscores = createMenu("Highscores", mainMenu, HIGSCORES);
	
	// Create a list of all the menu options in menu1 - PLAY GAME
		
	// Create a list of all the menu options in menu2 - OPTIONS
	menu_t* difficulty = createMenu("Difficulty", options,DIFFICULTY);
	menu_t* brightness = createMenu("Brightness", options, BRIGHTNESS);
	
	// Create a list of all the menu options in menu3 - HIGHSCORES
	menu_t* test1 = createMenu("TEST1", highscores, TEST1);
	menu_t* test2 = createMenu("TEST2", highscores, TEST2);
	menu_t* test3 = createMenu("TEST3", highscores, TEST3);
	menu_t* test4 = createMenu("TEST4", highscores, TEST4);	
		
	// Link Main Menu-options together to form a list - firstChild then rightSibling
	setFirstChild(mainMenu, playgame);
	setRightSibling(playgame, options);
	setRightSibling(options, highscores);
	
	// Link Menu 1 options together to form a list - firstChild then rightSibling
	
	// Link Menu 2 options together to form a list - firstChild then rightSibling
	setFirstChild(options, difficulty);
	setRightSibling(difficulty, brightness);
	
	// Link Menu 3 options together to form a list - firstChild then rightSibling
	setFirstChild(highscores, test1);
	setRightSibling(test1, test2);
	setRightSibling(test2, test3);
	setRightSibling(test3, test4);
		
	currentMenu = mainMenu;
	drawMenu(currentMenu);
	
	
	
}

int menuLength(menu_t* menu)
{
	int number = 0;
	
	menu = menu->firstChild;
	
	if (menu == NULL) return number;
	
	while (menu != NULL)
	{
		number++;
		menu = menu->rightSibling;
	}
	
	return number;
}


void drawMenu(menu_t* menu)	// pointer to the selected function
{

	int line = 0;
	OLEDClearAll();
	OLEDHome();
	OLEDPrintf(menu->title);
	
	menu = menu->firstChild;
	int menuWidth = strlen(menu->title)*8;

	while(menu != NULL)
	{
		line++;
		
		if (line == currentLine)
			{
				OLEDGotoPosition(currentLine,2);
				OLEDprintArrowRight();
				//OLEDGotoPosition(currentLine,strlen(menu->title)*8);
				OLEDinvPrintf(menu->title);
				OLEDprintArrowLeft();
			}
		else
			{
				OLEDGotoPosition(line,10);
				OLEDPrintf(menu->title);
			}
		menu = menu->rightSibling;
	}
	
	
}

void gotoMenuFunction(menu_t* menu)
{
	//printf("%s - %d",currentMenu->title, currentMenu->ID);
	switch ((int)currentMenu->ID)
	{
		case MAIN_MENU: printf("Main Menu function"); break;
		case PLAYGAME: printf("play game"); game(); break;
		case TEST1: printf("test1 function"); break;
		case BRIGHTNESS: printf("Brightness"); OLEDContrast(); break;  
		default: printf("Unknown Menu"); break;
	}
}


selectedMenu_t selectMenu()
{
	int lengthOfMenu = menuLength(currentMenu);
	joystickDriver();
	switch(joystick_data.joystickPosition)
	{
		case UP:
			currentLine--;
			if(currentLine <= 0)
				currentLine = lengthOfMenu;
			drawMenu(currentMenu);	
			break;
		case DOWN:
			currentLine++;
			if(currentLine >= lengthOfMenu+1)
				currentLine = 1;
			drawMenu(currentMenu);
			break;
		
		case LEFT:
			if(currentMenu->parent != NULL)
				currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			
			break;
		case RIGHT:
			if (currentMenu->firstChild != NULL)
			{
				currentMenu = currentMenu->firstChild;
				for(int i = 1; i < currentLine; i++)
					currentMenu = currentMenu->rightSibling;
					
				currentLine = 1; // sets the cursor to the first line in the new menu window
				drawMenu(currentMenu);
			}	
			break;
		default:
			// If direction = CENTER - Do nothing
		if(joystickButtonInterrupt == 1 && (currentMenu->firstChild == NULL))
			{	joystickButtonInterrupt = 0; gotoMenuFunction(&currentMenu);	}
			
		return currentLine;
		
	}
	
	
}