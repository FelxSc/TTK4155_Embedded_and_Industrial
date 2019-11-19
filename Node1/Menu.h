/*
 * Menu.h
 *
 * Created: 02.10.2019 13:59:22
 *  Author: Odd Inge Halsos & Felix Sch�pe
 */ 

#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <stdlib.h>


typedef enum{
	MAIN_MENU, GAME, OPTIONS, MUSIC, PLAYGAME, DIFFICULTY, EASY, NORMAL, HARD, HIGHSCORE, BRIGHTNESS, CHANGEFONT, PIDTUNER, RESETHIGHSCORE, CREDITS, RESETSCORE, ZELDA, GOT, FONTSMALL, FONTMEDIUM, FONTBIG
} selectedMenu_t;

// Menu structure as the LeftChild-RightSibling (LCRS)
typedef struct{
	char* title;
	selectedMenu_t* ID;
	struct menu_t* parent;
	struct menu_t* firstChild;
	struct menu_t* rightSibling;
}menu_t;



void menuInit( void );
void selectMenu( void );
void drawMenu(menu_t* menu);


#endif // MENU_H