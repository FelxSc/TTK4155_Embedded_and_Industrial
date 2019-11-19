//#include "OLED.h"
#include "ADC.h"
#include "JOYSTICK.h"
#include "Menu.h"
#include "Interrupt.h"
#include "game.h"
#include "OLED.h"
#include "Music.h"
#include "HIGHSCORE.h"

#include <avr/io.h>
#include <avr/common.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
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
	// Create list of menu options in Main Menu
	menu_t* mainMenu = createMenu("Main Menu", NULL, MAIN_MENU);
	menu_t* game = createMenu("Game", mainMenu, GAME);
	menu_t* options = createMenu("Options", mainMenu, OPTIONS);
	menu_t* graphics = createMenu("Graphics??", mainMenu, GRAPHICS);
	menu_t* music = createMenu("Music", mainMenu, MUSIC);
	
	// Create list of menu options in Game
	menu_t* playgame = createMenu("Play Game", game, PLAYGAME);
	menu_t* difficulty = createMenu("Difficulty", game, DIFFICULTY);
	menu_t* highscore = createMenu("Highscore", game, HIGHSCORE);
	
	// Create list of menu options in options
	menu_t* brightness = createMenu("Brightness", options, BRIGHTNESS);
	menu_t* changeFont = createMenu("Change Font", options, CHANGEFONT);
	menu_t* PIDtuner = createMenu("PID Tuner", options, PIDTUNER);
	menu_t* resethighscore = createMenu("Reset Highsc.", options, RESETHIGHSCORE);
	menu_t* credits = createMenu("Credits", options, CREDITS);
	
	// Create list of menu options in Music
	menu_t* zelda = createMenu("Zelda", music, ZELDA);
	menu_t* got = createMenu("GoT", music, GOT);
	
	// Create list of menu options in ChangeFont
	menu_t* small = createMenu("Small", changeFont, FONTSMALL);
	menu_t* medium = createMenu("Medium", changeFont, FONTMEDIUM);
	menu_t* big = createMenu("Big", changeFont, FONTBIG);	
	
		
	// Link Main Menu - firstChild then rightSibling
	setFirstChild(mainMenu, game);
	setRightSibling(game, options);
	setRightSibling(options, graphics);
	setRightSibling(graphics, music);
	
	// Link Game - firstChild then rightSibling
	setFirstChild(game, playgame);
	setRightSibling(playgame, difficulty);
	setRightSibling(difficulty, highscore);

	// Link Options - firstChild then rightSibling
	setFirstChild(options, brightness);
	setRightSibling(brightness, changeFont);
	setRightSibling(changeFont, PIDtuner);
	setRightSibling(PIDtuner, resethighscore);
	setRightSibling(resethighscore, credits);
	
	// Link Music - firstChild then rightSibling
	setFirstChild(music, zelda);
	setRightSibling(zelda, got);
	
	// Link Music - firstChild then rightSibling
	setFirstChild(changeFont, small);
	setRightSibling(small, medium);
	setRightSibling(medium, big);
	
		
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
	fprintf(OLED_p, menu->title);
	
	menu = menu->firstChild;
	int menuWidth = strlen(menu->title)*8;

	while(menu != NULL)
	{
		line++;
		
		if (line == currentLine)
			{
				OLEDGotoPosition(currentLine,2);
				OLEDprintArrowRight();
				fprintf(OLED_invp, menu->title);
				OLEDprintArrowLeft();
			}
		else
			{
				OLEDGotoPosition(line,10);
				fprintf(OLED_p, menu->title);
			}
		menu = menu->rightSibling;
	}

}

void gotoMenuFunction(menu_t* menu)
{
	//printf("%s - %d",currentMenu->title, currentMenu->ID);
	switch ((int)currentMenu->ID)
	{
		case MAIN_MENU: break;
		// ------GAME------
		// Playgame
		case PLAYGAME: game(); break;
		// Highscore
		case HIGHSCORE:
			OLEDShowHighscore();
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		// ------OPTIONS------
		// Brightness
		case BRIGHTNESS:
			OLEDContrast(); 
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;  
		// Change Font
		case FONTSMALL:
			setFontSize(4);
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		case FONTMEDIUM:
			setFontSize(5);
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		case FONTBIG:
			setFontSize(8);
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		// Credits
		case CREDITS:
			OLEDGotoPosition(1,0);
			fprintf(OLED_p, "Created by");
			OLEDGotoPosition(2,0);
			fprintf(OLED_p, "Odd Inge Halsos");
			OLEDGotoPosition(3,0);
			fprintf(OLED_p, "Felix Schoepe");
			break;
		// PIDTUNER
		case PIDTUNER:
			break;
		case RESETHIGHSCORE:
			resetHighscore();
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		// ------MUSIC------
		case ZELDA:
			OLEDGotoPosition(3,24);
			fprintf(OLED_p, "Listen :-)");
			PlaySong(ZELDA_ID);
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);
			break;
		case GOT:
			OLEDGotoPosition(3,24);
			fprintf(OLED_p, "Listen :-)");
			PlaySong(GOT_ID);
			currentMenu = currentMenu->parent;
			drawMenu(currentMenu);	
			break;


		default: break;
	}
}


void selectMenu()
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
		default: break; // If direction = CENTER - Do nothing
		
		


	}
		
		if(joystickButtonInterrupt == 1 && (currentMenu->firstChild == NULL))
		{	joystickButtonInterrupt = 0; gotoMenuFunction(&currentMenu);	}
		
		return currentLine;
		
}